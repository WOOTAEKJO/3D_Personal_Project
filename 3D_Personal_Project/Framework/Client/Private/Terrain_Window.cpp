#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Public/Terrain_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"
#include "Json/Json_Utility.h"
#include "Terrain_Demo.h"
#include "ObjectMesh_Demo.h"

#include "DebugDraw.h"

#include "Cell.h"

CTerrain_Window::CTerrain_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CImGui_Window(pDevice, pContext)
{
}

HRESULT CTerrain_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t		iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;

	m_vecSphere.clear();

	return S_OK;
}

void CTerrain_Window::Tick()
{
	Terrain_Update();
	Navigation_Update();
	
}

HRESULT CTerrain_Window::Render()
{
	CImGui_Window::Begin();

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Terrain", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("HeightMap"))
		{
			m_eCurrentMode = MODE_HEIGHT;

			HeightMap();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Navigation"))
		{
			m_eCurrentMode = MODE_NAVIGATION;

			Navigation();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	CImGui_Window::End();

	return S_OK;
}

void CTerrain_Window::Set_Variable(void* pArg)
{
	if (pArg == nullptr)
		return;
}

void CTerrain_Window::Terrain_Picked(_float4 vPickPoint)
{
	if (m_pTerrain == nullptr)
		return;

   	m_vPickPos = vPickPoint;

	if (m_eCurrentMode == MODE_HEIGHT) {
		m_pTerrain->Update_HeightMap(XMLoadFloat4(&vPickPoint), (_float)m_iHeight_Control[0], (_float)m_iHeight_Control[1], m_fSharpness);
	}
	else if (m_eCurrentMode == MODE_NAVIGATION)
	{
		if (Set_NaviPickPos()) {
			
		}
	}
}

void CTerrain_Window::Demo_Picked()
{
	if (m_eCurrentMode == MODE_NAVIGATION) {
		if (m_vecSphere.empty())
			return;

		if (m_pGameInstance->Mouse_Down(DIM_RB)) {

			for (size_t i = 0; i < m_vecSphere.size(); i++)
			{
				_float fDist = 0.f;

				if (m_pGameInstance->Intersect_Sphere(m_vecSphere[i], &fDist))
				{
					m_iCurrentSphereIndex = i;
				}
			}
		}

		Fix_Navigation();
	}
}

HRESULT CTerrain_Window::Save_Data(const _char* strFilePath)
{
	if (m_pTerrain == nullptr)
		return E_FAIL;

	if (m_eCurrentMode == TERRAINMODE::MODE_HEIGHT) {

		if (FAILED(m_pTerrain->Save_Terrain(strFilePath)))
			return E_FAIL;
	}
	else if (m_eCurrentMode == TERRAINMODE::MODE_NAVIGATION)
	{
		if (FAILED(m_pTerrain->Save_Navigation(strFilePath)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrain_Window::Load_Data(const _char* strFilePath)
{
	if (m_pTerrain == nullptr)
		return E_FAIL;

	if (m_eCurrentMode == TERRAINMODE::MODE_HEIGHT) {

		if (FAILED(m_pTerrain->Load_Terrain(strFilePath)))
			return E_FAIL;
	}
	else if (m_eCurrentMode == TERRAINMODE::MODE_NAVIGATION)
	{
		for (auto& iter : m_vecSphere)
			Safe_Delete(iter);
		m_vecSphere.clear();
		m_vecCell.clear();

		if (FAILED(m_pTerrain->Load_Navigation(strFilePath)))
			return E_FAIL;

		vector<CCell*> vecCells = m_pTerrain->Get_Navigation_Cells();

		_uint iSize = vecCells.size();
		for (_uint i = 0; i < iSize; i++)
		{
			NAVI_CELL_DESC NaviCellDesc = {};

			for (_uint j = 0; j < 3; j++) {

				_bool bCheck = true;

				_uint iSSize = m_vecSphere.size();
				for (_uint k=0;k<iSSize;k++)
				{
					if (XMVector3Equal(XMLoadFloat3(&m_vecSphere[k]->Center), XMLoadFloat3(&vecCells[i]->Get_Point((CCell::POINTS)j))))
					{
						NaviCellDesc.iSphereIndex[j] = k;
						bCheck = false;
						break;
					}
				}

				if (bCheck)
				{
					BoundingSphere* pSphere = new BoundingSphere(vecCells[i]->Get_Point((CCell::POINTS)j), 0.5f);
					NaviCellDesc.iSphereIndex[j] = m_vecSphere.size();
					m_vecSphere.push_back(pSphere);
				}
			}

			NaviCellDesc.iCellIndex = i;
			m_vecCell.push_back(NaviCellDesc);
		}
	}

	return S_OK;
}

void CTerrain_Window::HeightMap()
{
	ImGui::InputInt2("Vertices_SizeX,Z", m_iVertices_Size);

	if (ImGui::Button("Create_HeightMap"))
	{
		if (m_iVertices_Size[0] < 2 || m_iVertices_Size[1] < 2) {
			MSG_BOX("The number is too small");
			return;
		}

		Create_HeightMap();
	}

	ImGui::SliderInt("Radius", &m_iHeight_Control[0], 0, 30);
	ImGui::SliderInt("Height", &m_iHeight_Control[1], 0, 30);
	ImGui::SliderFloat("Sharpness", &m_fSharpness, 0.f, 1.f);

	ImGui::Checkbox("WireFrame", &m_bWireFrame);

	ImGui::Text(to_string(m_vPickPos.x).c_str());
}

void CTerrain_Window::Navigation()
{
	ImVec2 vSize = ImVec2(250, 100);
	ImGui::BeginListBox("Points", vSize);
	_uint iSize = m_vecSphere.size();
	for (_uint i = 0; i < iSize; i++)
	{
		string str = to_string(i);
		if (ImGui::Selectable(str.c_str(), i == m_iCurrentSphereIndex)) {
			m_iCurrentSphereIndex = i;
		}
	}
	ImGui::EndListBox();

	ImGui::RadioButton("Sphere", &m_iCurrentNaviModeRadioButton, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Cell", &m_iCurrentNaviModeRadioButton, 1);

	if (!m_vecSphere.empty() && m_vecSphere[m_iCurrentSphereIndex] != nullptr) {

		__super::ImGuizmo(ImGuizmo::MODE::WORLD, &m_vecSphere[m_iCurrentSphereIndex]->Center);
	}

	Sphere_Render();
}

void CTerrain_Window::Navigation_Update()
{
	if (m_vNaviPos[0].bCheck && m_vNaviPos[1].bCheck && m_vNaviPos[2].bCheck)
	{
		_float3 vPoints[3] = { m_vNaviPos[0].vPosition,m_vNaviPos[1].vPosition,m_vNaviPos[2].vPosition };

		NAVI_CELL_DESC NaviCellDesc = {};

		NaviCellDesc.iSphereIndex[0] = m_vNaviPos[0].iSphereIndex;
		NaviCellDesc.iSphereIndex[1] = m_vNaviPos[1].iSphereIndex;
		NaviCellDesc.iSphereIndex[2] = m_vNaviPos[2].iSphereIndex;

        m_pTerrain->Add_Navigation_Cell(vPoints, &NaviCellDesc.iCellIndex);
		m_vecCell.push_back(NaviCellDesc);
		Reset_NaviPickPos();

	}
}

_bool CTerrain_Window::Set_NaviPickPos()
{
	for (_uint i = 0; i < 3; i++)
	{
		if (!m_vNaviPos[i].bCheck)
		{
			m_vNaviPos[i].bCheck = true;
			_uint iSize = m_vecSphere.size();
			for (_uint j = 0; j < iSize; j++)
			{
				_float fDist = 0.f;
				if (m_pGameInstance->Intersect_Sphere(m_vecSphere[j], &fDist))
				{
					m_vNaviPos[i].vPosition = m_vecSphere[j]->Center;
					m_vNaviPos[i].iSphereIndex = j;
					return false;
				}
			}

			m_vNaviPos[i].vPosition = _float3(m_vPickPos.x, m_vPickPos.y + 0.001f, m_vPickPos.z);
     		BoundingSphere* pSphere = new BoundingSphere(m_vNaviPos[i].vPosition, 0.5f);
			m_vNaviPos[i].iSphereIndex = m_vecSphere.size();
			m_vecSphere.push_back(pSphere);
			return false;
		}
	}

	return true;
}

void CTerrain_Window::Reset_NaviPickPos()
{
	for (_uint i = 0; i < 3; i++)
	{
		m_vNaviPos[i].bCheck = false;
		m_vNaviPos[i].vPosition = _float3(0.f,0.f,0.f);
		m_vNaviPos[i].iSphereIndex = 0;
	}
}

void CTerrain_Window::Sphere_Render()
{
	if (m_vecSphere.empty())
		return;

	_float4 vColor(1.f, 0.f, 1.f, 0.f);

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::PROJ));

	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	for (auto& iter : m_vecSphere)
	{
		DX::Draw(m_pBatch, *iter, XMLoadFloat4(&vColor));
	}

	m_pBatch->End();
}

void CTerrain_Window::Fix_Navigation()
{
	switch (m_iCurrentNaviModeRadioButton)
	{
	case 0:

		for (auto& iter : m_vecCell)
		{
			for (_uint i = 0; i < 3; i++) {
				if (iter.iSphereIndex[i] == m_iCurrentSphereIndex)
				{
					FLOAT3X3 Flaot33 = {};
					Flaot33.vVertex0 = m_vecSphere[iter.iSphereIndex[0]]->Center;
					Flaot33.vVertex1 = m_vecSphere[iter.iSphereIndex[1]]->Center;
					Flaot33.vVertex2 = m_vecSphere[iter.iSphereIndex[2]]->Center;

					m_pTerrain->Update_Navigation_Cell(iter.iCellIndex, Flaot33);
				}
			}
		}

		break;
	case 1:
		break;
	}
}

void CTerrain_Window::Create_HeightMap()
{
	if (m_pTerrain == nullptr)
		        return;
		
	if (FAILED((m_pTerrain->Create_DynamicBuffer(m_iVertices_Size[0], m_iVertices_Size[1]))))
	    return ;
}

void CTerrain_Window::Terrain_Update()
{
	CTerrain_Demo::TERRAINDEMOVALUE tTerrainDemoValue;

	tTerrainDemoValue.fRadius = (_float)m_iHeight_Control[0];
	tTerrainDemoValue.bWireFrame = m_bWireFrame;

	m_pTerrain->Set_Control_Variable(&tTerrainDemoValue);
}

CTerrain_Window* CTerrain_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg)
{
	CTerrain_Window* pInstance = new CTerrain_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTerrain_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain_Window::Free()
{
	__super::Free();

	for (auto& iter : m_vecSphere)
		Safe_Delete(iter);
	m_vecSphere.clear();

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Release(m_pInputLayout);
}

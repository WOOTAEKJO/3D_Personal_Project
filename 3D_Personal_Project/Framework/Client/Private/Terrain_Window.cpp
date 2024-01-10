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

	m_vecDemo = CImGuiMgr::GetInstance()->Get_ObjectDemo();

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

	if (m_eCurrentMode == MODE_HEIGHT) {

		m_vPickPos = vPickPoint;
		m_pTerrain->Update_HeightMap(XMLoadFloat4(&vPickPoint), (_float)m_iHeight_Control[0], (_float)m_iHeight_Control[1], m_fSharpness);
	}
	else if (m_eCurrentMode == MODE_NAVIGATION)
	{
		if (m_iNaviWorkType == 0)
		{
			m_vPickPos = vPickPoint;
			Set_NaviPickPos();
		}else if (m_iNaviWorkType == 2)
		{
			m_vPickPos = vPickPoint;
			Add_Neighbor();
		}
	}
}

void CTerrain_Window::Demo_Picked()
{
	if (m_eCurrentMode == MODE_NAVIGATION) {

		if (m_iNaviWorkType == 1)
		{
			if (m_pGameInstance->Key_Pressing(DIK_SPACE)) {
				if (m_pGameInstance->Mouse_Down(DIM_RB)) {

					_uint iSize = m_vecDemo->size();
					_float fMinDist = 1000.f;
					_float fDist = 0.f;
					_float4 vPos = {};
					for (size_t i = 0; i < iSize; i++)
					{
  						if ((*m_vecDemo)[i]->Get_Picked_Dist(&vPos, &fDist))
						{
							if (fMinDist > fDist)
							{
								fMinDist = fDist;
								m_vPickPos = vPos;
							}
						}
					}

					Set_NaviPickPos();
				}
			}
		}
		else {
			if (m_vecSphere.empty())
				return;

			if (m_pGameInstance->Mouse_Down(DIM_RB)) {

				Picked_Navigation();
			}

			Fix_Navigation();
		}

	}
}

string CTerrain_Window::Get_Path()
{
	if (m_eCurrentMode == TERRAINMODE::MODE_HEIGHT)
		return "../Bin/Data/Terrain/";
	else if(m_eCurrentMode == TERRAINMODE::MODE_NAVIGATION)
		return "../Bin/Data/Terrain/Navigation/";
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
					BoundingSphere* pSphere = new BoundingSphere(vecCells[i]->Get_Point((CCell::POINTS)j), 0.1f);
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

	if (ImGui::Checkbox("ADD", &m_bAdd))
	{
		m_pTerrain->Set_Add(m_bAdd);
	}

	ImGui::Checkbox("WireFrame", &m_bWireFrame);

	ImGui::Text(to_string(m_vPickPos.x).c_str());
}

void CTerrain_Window::Navigation()
{
	ImVec2 vSize = ImVec2(250, 100);
	ImGui::BeginListBox("Sphere", vSize);
	_uint iSize = m_vecSphere.size();
	for (_uint i = 0; i < iSize; i++)
	{
		string str = to_string(i);
		if (ImGui::Selectable(str.c_str(), i == m_iCurrentSphereIndex)) {
			m_iCurrentSphereIndex = i;
		}
	}
	ImGui::EndListBox();

	ImGui::BeginListBox("Cell", vSize);
	iSize = m_vecCell.size();
	for (_uint i = 0; i < iSize; i++)
	{
		string str = to_string(i);
		if (ImGui::Selectable(str.c_str(), i == m_iCurrentCellIndex)) {
			m_iCurrentCellIndex = i;
		}
	}
	ImGui::EndListBox();

	ImGui::RadioButton("None", &m_iCurrentNaviModeRadioButton, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Sphere", &m_iCurrentNaviModeRadioButton, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Cell", &m_iCurrentNaviModeRadioButton, 2);

	ImGui::RadioButton("Terrain", &m_iNaviWorkType, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Mesh", &m_iNaviWorkType, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Add_Neighbor", &m_iNaviWorkType, 2);


	if (!m_vecCell.empty())
	{
		string strNaviInd = "NaviIndex : " + to_string(m_iCurrentCellIndex);
		ImGui::Text(strNaviInd.c_str());

		ImGui::Text((to_string(m_vecCell[m_iCurrentCellIndex].iSphereIndex[0])+ " ").c_str());
		ImGui::SameLine();
		ImGui::Text((to_string(m_vecCell[m_iCurrentCellIndex].iSphereIndex[1]) + " ").c_str());
		ImGui::SameLine();
		ImGui::Text((to_string(m_vecCell[m_iCurrentCellIndex].iSphereIndex[2]) + " ").c_str());

		ImGui::Text((to_string(m_pTerrain->Get_Navigation_Cells()[m_iCurrentCellIndex]
			->Get_NeighborIndex((CCell::LINES)0))+ " ").c_str());
		ImGui::SameLine();
		ImGui::Text((to_string(m_pTerrain->Get_Navigation_Cells()[m_iCurrentCellIndex]
			->Get_NeighborIndex((CCell::LINES)1)) + " ").c_str());
		ImGui::SameLine();
		ImGui::Text((to_string(m_pTerrain->Get_Navigation_Cells()[m_iCurrentCellIndex]
			->Get_NeighborIndex((CCell::LINES)2)) + " ").c_str());

		ImGui::RadioButton("Normal", &m_iCellType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Jump", &m_iCellType, 1);

		if (m_iCellType == 1)
		{
			Arrow_Button("Interval", 0.1f, &m_fJumpCellHeight);
		}
	}

	ImGui::Checkbox("Trans", &m_bNeviPosTrans);
	if(m_bNeviPosTrans){
		if (!m_vecSphere.empty() && m_vecSphere[m_iCurrentSphereIndex] != nullptr) {

			__super::ImGuizmo(ImGuizmo::MODE::WORLD, &m_vecSphere[m_iCurrentSphereIndex]->Center);
		}
	}

	if (m_iNaviWorkType == 2)
	{
		for (_uint i = 0; i < 2; i++)
		{
			string strTag = i == 0 ? "SourCell :" : "DestCell :";

			ImGui::Text((strTag + to_string(m_iSelectedCellIdx[i])).c_str());

			ImGui::Text(("Sphere1 :" + to_string(m_eSelectedSphere[0 + (i * 2)].vPosition.x) + " " +
				to_string(m_eSelectedSphere[0 + (i * 2)].vPosition.y) + 
				" " + to_string(m_eSelectedSphere[0 + (i * 2)].vPosition.z)).c_str());
			ImGui::Text(("Sphere2 :" + to_string(m_eSelectedSphere[1 + (i * 2)].vPosition.x) + " " +
				to_string(m_eSelectedSphere[1 + (i * 2)].vPosition.y) + 
				" " + to_string(m_eSelectedSphere[1 + (i * 2)].vPosition.z)).c_str());
		}

		if (m_iSelectedCellIdx[0] != -1 && m_eSelectedSphere[0].bCheck && m_eSelectedSphere[1].bCheck &&
			m_iSelectedCellIdx[1] != -1 && m_eSelectedSphere[2].bCheck && m_eSelectedSphere[3].bCheck)
		{
			_float3 pSour[2] = { m_eSelectedSphere[0].vPosition, m_eSelectedSphere[1].vPosition };
			_float3 pDest[2] = { m_eSelectedSphere[2].vPosition, m_eSelectedSphere[3].vPosition };

			m_pTerrain->Add_Neighbor(m_iSelectedCellIdx[0], pSour, m_iSelectedCellIdx[1], pDest);

			for (_uint i = 0; i < 2; i++)
			{
				m_iSelectedCellIdx[i] = -1;
				m_eSelectedSphere[0 + (i * 2)].bCheck = false;
				m_eSelectedSphere[0 + (i * 2)].vPosition = _float3(0.f, 0.f, 0.f);
				m_eSelectedSphere[1 + (i * 2)].bCheck = false;
				m_eSelectedSphere[1 + (i * 2)].vPosition = _float3(0.f, 0.f, 0.f);
			}
		}
	}

	/*if (ImGui::Button("All_Delete"))
	{
		All_Delete_Cell();
	}*/

	if (m_iCurrentNaviModeRadioButton == 2) {
		
		if (ImGui::Button("Change Cell Type"))
		{
			m_pTerrain->Set_Cell_Type(m_iCellType, m_iCurrentCellIndex);
		}

		if (ImGui::Button("Selected_Delete"))
		{
			Selected_Delete_Cell();
		}
	}

	Sphere_Render();
}

void CTerrain_Window::Navigation_Update()
{
	if (m_vNaviPos[0].bCheck && m_vNaviPos[1].bCheck && m_vNaviPos[2].bCheck)
	{
	
		Calculate_Cell();

		for (_uint i = 0; i < 3; i++)
		{
			if (m_iCalculate[i] == -1)
			{
				Reset_NaviPickPos();
				MSG_BOX("Cell Create Failed");
				return;
			}
		}

		_float3 vPoints[3] = { m_vNaviPos[m_iCalculate[0]].vPosition,
			m_vNaviPos[m_iCalculate[1]].vPosition,m_vNaviPos[m_iCalculate[2]].vPosition };

		NAVI_CELL_DESC NaviCellDesc = {};

		NaviCellDesc.iSphereIndex[0] = m_vNaviPos[m_iCalculate[0]].iSphereIndex;
		NaviCellDesc.iSphereIndex[1] = m_vNaviPos[m_iCalculate[1]].iSphereIndex;
		NaviCellDesc.iSphereIndex[2] = m_vNaviPos[m_iCalculate[2]].iSphereIndex;

        m_pTerrain->Add_Navigation_Cell(vPoints, &NaviCellDesc.iCellIndex,m_iCellType);
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
					if (m_iCellType == 0) // 셀이 Normal 타입일 때
					{
						m_vNaviPos[i].vPosition = m_vecSphere[j]->Center;
						m_vNaviPos[i].iSphereIndex = j;
						return false;
					}
					else if (m_iCellType == 1) // 셀이 Jump 타입일 때
					{
						if (m_fJumpCellHeight == 0.f)
						{
							m_vNaviPos[i].vPosition = m_vecSphere[j]->Center;
							m_vNaviPos[i].iSphereIndex = j;
							return false;
						}
						else
						{
							m_vNaviPos[i].vPosition = _float3(m_vecSphere[j]->Center.x,
								m_vecSphere[j]->Center.y + 0.001f + m_fJumpCellHeight,
								m_vecSphere[j]->Center.z);
							BoundingSphere* pSphere = new BoundingSphere(m_vNaviPos[i].vPosition, 0.1f);
							m_vNaviPos[i].iSphereIndex = m_vecSphere.size();
							m_vecSphere.push_back(pSphere);
							return false;
						}
					}
				}
			}

			m_vNaviPos[i].vPosition = _float3(m_vPickPos.x, m_vPickPos.y + 0.001f, m_vPickPos.z);
     		BoundingSphere* pSphere = new BoundingSphere(m_vNaviPos[i].vPosition, 0.1f);
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
		m_iCalculate[i] = -1;
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

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	for (auto& iter : m_vecSphere)
	{
		DX::Draw(m_pBatch, *iter, XMLoadFloat4(&vColor));
	}

	m_pBatch->End();
}

void CTerrain_Window::Calculate_Cell()
{
	_int a(0), b(1), c(2);

	for (_uint i = 0; i < 3; i++) {
		
		_float iResult = (m_vNaviPos[b].vPosition.x - m_vNaviPos[a].vPosition.x) * (m_vNaviPos[c].vPosition.z - m_vNaviPos[a].vPosition.z)
			- (m_vNaviPos[c].vPosition.x - m_vNaviPos[a].vPosition.x) * (m_vNaviPos[b].vPosition.z - m_vNaviPos[a].vPosition.z);

		if (0.f > iResult)
		{
			m_iCalculate[0] = a;
			m_iCalculate[1] = b;
			m_iCalculate[2] = c;

			return;
		}else if(0.f < iResult)
		{
			m_iCalculate[0] = a;
			m_iCalculate[1] = c;
			m_iCalculate[2] = b;

			return;
		}

		a -= 1;
		if (a < 0)
			a = 2;
		b -= 1;
		if (b < 0)
			b = 2;
		c -= 1;
		if (c < 0)
			c = 2;
	}

}

void CTerrain_Window::Fix_Navigation()
{
	switch (m_iCurrentNaviModeRadioButton)
	{
	case 0:
		break;
	case 1:

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
	case 2:
		break;
	}
}

void CTerrain_Window::Picked_Navigation()
{
	switch (m_iCurrentNaviModeRadioButton)
	{
	case 0:
		break;
	case 1:
		for (size_t i = 0; i < m_vecSphere.size(); i++)
		{
			_float fDist = 0.f;

			if (m_pGameInstance->Intersect_Sphere(m_vecSphere[i], &fDist))
			{
				m_iCurrentSphereIndex = i;
			}
		}
		break;
	case 2:
	{
		_uint iCellIndex = 0;
		if (m_pTerrain->Picked_Cell(&iCellIndex))
		{
			m_iCurrentCellIndex = iCellIndex;
		}
	}
		break;
	}
}

void CTerrain_Window::All_Delete_Cell()
{
	if (m_pTerrain == nullptr)
		return;

	m_pTerrain->All_Delete_Cell();

	m_vecCell.clear();

	for (auto& iter : m_vecSphere)
		Safe_Delete(iter);
	m_vecSphere.clear();

	m_iCurrentCellIndex = 0;
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

void CTerrain_Window::Selected_Delete_Cell()
{
	if (m_iCurrentCellIndex >= m_vecCell.size())
		return;

	m_pTerrain->Selected_Delete_Cell(m_iCurrentCellIndex);

	NAVIDEMO_DESC SphereIndex[3] = {};

	SphereIndex[0].iSphereIndex = m_vecCell[m_iCurrentCellIndex].iSphereIndex[0];
	SphereIndex[1].iSphereIndex = m_vecCell[m_iCurrentCellIndex].iSphereIndex[1];
	SphereIndex[2].iSphereIndex = m_vecCell[m_iCurrentCellIndex].iSphereIndex[2];

	for (_uint i = 0; i < 3; ++i)
	{
		for (_uint j = i + 1; j < 3; ++j)
		{
			if (SphereIndex[i].iSphereIndex < SphereIndex[j].iSphereIndex)
			{
				_uint iTmp;
				iTmp = SphereIndex[j].iSphereIndex;
				SphereIndex[j].iSphereIndex = SphereIndex[i].iSphereIndex;
				SphereIndex[i].iSphereIndex = iTmp;
			}
		}
	}

	m_vecCell.erase(m_vecCell.begin() + m_iCurrentCellIndex);

	for (auto& iter : m_vecCell)
	{
		if (iter.iCellIndex > m_iCurrentCellIndex)
		{
			iter.iCellIndex -= 1;
		}

		for (_uint i = 0; i < 3; i++)
		{
			if (iter.iSphereIndex[i] == SphereIndex[0].iSphereIndex)
				SphereIndex[0].bCheck = true;
			if (iter.iSphereIndex[i] == SphereIndex[1].iSphereIndex)
				SphereIndex[1].bCheck = true;
			if (iter.iSphereIndex[i] == SphereIndex[2].iSphereIndex)
				SphereIndex[2].bCheck = true;
		}
	}

	for (_uint i = 0; i < 3; i++)
	{
		if (!SphereIndex[i].bCheck)
		{
			for (auto& iter : m_vecCell)
			{
				for (_uint j = 0; j < 3; j++)
				{
					if (iter.iSphereIndex[j] > SphereIndex[i].iSphereIndex)
					{
						iter.iSphereIndex[j] -= 1;
					}
				}
			}

			_uint iIndx = SphereIndex[i].iSphereIndex;
			if ((iIndx >= m_vecSphere.size()))
			{
				iIndx -= 1;
			}

			Safe_Delete(m_vecSphere[iIndx]);
			m_vecSphere.erase(m_vecSphere.begin() + iIndx);
		}
	}
	if(m_iCurrentCellIndex != 0)
		m_iCurrentCellIndex -= 1;
}

void CTerrain_Window::Add_Neighbor()
{

	_uint iIdnx = 0;

	if (m_iSelectedCellIdx[0] != -1 && m_eSelectedSphere[0].bCheck && m_eSelectedSphere[1].bCheck)
		++iIdnx;

	if(m_iSelectedCellIdx[iIdnx] == -1)
		m_iSelectedCellIdx[iIdnx] = m_iCurrentCellIndex;
	else {
		for (_uint i = iIdnx * 2; i < i + 2; i++)
		{
			if (!m_eSelectedSphere[i].bCheck)
			{
				_float fDist = 0.f;

				for (auto& iter : m_vecSphere)
				{
					if (m_pGameInstance->Intersect_Sphere(iter, &fDist))
					{
						for (_uint j = 0; j < 3; j++)
						{
							if (XMVector3Equal(XMLoadFloat3(&iter->Center),
								XMLoadFloat3(&m_pTerrain->Get_Navigation_Cells()[m_iSelectedCellIdx[iIdnx]]
									->Get_Point((CCell::POINTS)j))))
							{
								m_eSelectedSphere[i].vPosition = iter->Center;
								m_eSelectedSphere[i].bCheck = true;
								return;
							}
						}
					}
				}

			}
		}
	}

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



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

CTerrain_Window::CTerrain_Window()
{
}

HRESULT CTerrain_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	wcscpy_s(m_szFile, DATA_TERRAIN_PATH);

	/*m_Test.iNumVerticesX = 3;
	m_Test.iNumverticesZ = 3;

	m_Test.tTerrainInfo = new TERRAININFO[9];
	
	for (_uint i = 0; i < 9; i++)
	{
		
		VTXMESH dd;
		dd.vPosition = _float3(0.f, 0.f, 0.f);
		dd.vTangent = _float3(0.f, 0.f, 0.f);
		dd.vNormal = _float3(0.f, 0.f, 0.f);
		dd.vTexCoord = _float2(0.f, 0.f);

		_char* szName = "test";

		wsprintfA(szName, "%d", i);

		m_Test.tTerrainInfo[i].tVertex = dd;
		m_Test.tTerrainInfo[i].szVertexName = szName;
	}

	for (_uint i = 0; i < 4; i++)
	{
		_uint3 aa;
		aa = _uint3(1, 1, 1);

		_char* szName = "Intest";

		wsprintfA(szName, "%d", i);

		m_Test.tTerrainInfo[i].tIndex = aa;
		m_Test.tTerrainInfo[i].szIndexName = szName;
	}*/

	return S_OK;
}

void CTerrain_Window::Tick()
{
	Terrain_Update();

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
			HeightMap();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Tile"))
		{


			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	CImGui_Window::End();

	m_szFile;

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
	
	m_pTerrain->Update_HeightMap(XMLoadFloat4(&vPickPoint), (_float)m_iHeight_Control[0], (_float)m_iHeight_Control[1], m_fSharpness);

	m_vPickPos = vPickPoint;
}

void CTerrain_Window::Demo_Picked()
{
}

HRESULT CTerrain_Window::Save_Data()
{
	/*json dd;
	CJson_Utility::Write_Float3(dd["test1"], _float3(1.f, 1.f, 1.f));
	CJson_Utility::Write_Float3(dd["test2"], _float3(2.f, 2.f, 2.f));
	if (FAILED(CJson_Utility::Save_Json("../Bin/Terrain.json", dd)))
		return E_FAIL;*/
	/*int a = 1;

	ofstream fout;

	fout.open("../Bin/Terrain.json");

	if (fout.is_open())
	{

		fout.write(reinterpret_cast<const char*>(&a), sizeof(a));

	}
	else
		return E_FAIL;

	fout.close();*/

	if (m_pTerrain == nullptr)
		return E_FAIL;

	if (FAILED(m_pTerrain->Save_Terrain("../Bin/Export/Debug/x64/Data/Terrain/33.dat")))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CTerrain_Window::Load_Data()
{
	/*_float3 ff;
	json ddd;
	CJson_Utility::Load_Json("../Bin/Terrain.json", ddd);

	CJson_Utility::Load_Float3(ddd["test1"], ff);
	ff;
	int a = 0;*/

	int a = 0;

	ifstream fIn;

	fIn.open("../Bin/Terrain.json");

	if (fIn.is_open())
	{

		fIn.read(reinterpret_cast<char*>(&a), sizeof(a));

	}
	else
		return E_FAIL;

	fIn.close();

	a;

	int b = 0;

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

CTerrain_Window* CTerrain_Window::Create(void* pArg)
{
	CTerrain_Window* pInstance = new CTerrain_Window();

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
}

#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Public/Terrain_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"
#include "Terrain_Demo.h"
#include "ObjectMesh_Demo.h"

CTerrain_Window::CTerrain_Window()
{
}

HRESULT CTerrain_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTerrain_Window::Tick()
{
	Terrain_Update();
	ObjectMesh_Update();
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

		if (ImGui::BeginTabItem("ObjectMesh"))
		{
			ObjectMesh();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Tile"))
		{


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

	TERRAINWINDOWDESC* TerrainWindowDesc = (TERRAINWINDOWDESC*)pArg;

	m_vPickPos = TerrainWindowDesc->vPickPos;
	m_bPicked = TerrainWindowDesc->bPicked;
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

		CImGuiMgr::GetInstance()->Create_HeightMap(m_iVertices_Size[0], m_iVertices_Size[1]);
	}

	ImGui::SliderInt("Radius", &m_iHeight_Control[0], 0, 30);
	ImGui::SliderInt("Height", &m_iHeight_Control[1], 0, 30);
	ImGui::SliderFloat("Sharpness", &m_fSharpness, 0.f, 1.f);

	ImGui::Checkbox("WireFrame", &m_bWireFrame);
}

void CTerrain_Window::ObjectMesh()
{
	if (m_bPicked) {
	
		CObjectMesh_Demo* pObject = nullptr;
	
		CObjectMesh_Demo::OBDEMOVALUE ObjectDemoValue;
	
		ObjectDemoValue.vPos = m_vPickPos;//CImGuiMgr::GetInstance()->Get_PickingMousePoint();
	
		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, TEXT("Tool"),
			TEXT("Prototype_GameObject_ObjectMesh_Demo"), &ObjectDemoValue, reinterpret_cast<CGameObject**>(&pObject))))
			return;
	
		CImGuiMgr::GetInstance()->Add_Demo("Test", pObject);
	}
	m_bPicked = false;
	
}

void CTerrain_Window::Terrain_Update()
{
	CTerrain_Demo::TERRAINDEMOVALUE tTerrainDemoValue;

	tTerrainDemoValue.fRadius = (_float)m_iHeight_Control[0];
	tTerrainDemoValue.fHeight = (_float)m_iHeight_Control[1];
	tTerrainDemoValue.fSharpness = m_fSharpness;
	tTerrainDemoValue.bWireFrame = m_bWireFrame;

	CImGuiMgr::GetInstance()->Set_Terrain_Variable(&tTerrainDemoValue);
}

void CTerrain_Window::ObjectMesh_Update()
{
	
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

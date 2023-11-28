#include "stdafx.h"
#include "../Public/imgui.h"
#include "../Public/imgui_impl_win32.h"
#include "../Public/imgui_impl_dx11.h"
#include "..\Public\Terrain_Window.h"

#include "ImGuiMgr.h"

#include "GameInstance.h"
#include "Terrain_Demo.h"

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
	CTerrain_Demo::TERRAINDEMOVALUE tTerrainDemoValue;

	tTerrainDemoValue.fRadius = m_iHeight_Control[0];
	tTerrainDemoValue.fHeight = m_iHeight_Control[1];
	tTerrainDemoValue.fSharpness = m_iHeight_Control[2];

	CImGuiMgr::GetInstance()->Set_Control_Variable(&tTerrainDemoValue);

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


			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	CImGui_Window::End();

	return S_OK;
}

void CTerrain_Window::HeightMap()
{
	ImGui::InputInt2("Vertices_SizeX,Z", m_iVertices_Size);

	if (ImGui::Button("Create_HeightMap"))
	{
		if (m_iVertices_Size[0] < 0 || m_iVertices_Size[1] < 0)
			return;

		CImGuiMgr::GetInstance()->Create_HeightMap(m_iVertices_Size[0], m_iVertices_Size[1]);
	}

	if (ImGui::Button("Delete_HeightMap"))
	{
		CImGuiMgr::GetInstance()->Delete_HeightMap();
	}

	ImGui::InputInt3("R_H_S", m_iHeight_Control);

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

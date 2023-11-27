#include "stdafx.h"
#include "..\Public\Terrain_Window.h"

#include "GameInstance.h"

CTerrain_Window::CTerrain_Window()
{
}

HRESULT CTerrain_Window::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTerrain_Window::Tick()
{
}

HRESULT CTerrain_Window::Render()
{
	__super::Begin();

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

	__super::End();

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

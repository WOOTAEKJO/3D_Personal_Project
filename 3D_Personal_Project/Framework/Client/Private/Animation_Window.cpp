#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "..\Public\Animation_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"

CAnimation_Window::CAnimation_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CImGui_Window(pDevice, pContext)
{
}

HRESULT CAnimation_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAnimation_Window::Tick()
{
}

HRESULT CAnimation_Window::Render()
{
	CImGui_Window::Begin();

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Animation", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Animation"))
		{
			Animation();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("SubObject"))
		{
			SubObject();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	CImGui_Window::End();

	return S_OK;
}

void CAnimation_Window::Set_Variable(void* pArg)
{
}

void CAnimation_Window::Terrain_Picked(_float4 vPickPoint)
{
}

void CAnimation_Window::Demo_Picked()
{
}

void CAnimation_Window::Animation()
{
}

void CAnimation_Window::SubObject()
{
	
}

CAnimation_Window* CAnimation_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg)
{
	CAnimation_Window* pInstance = new CAnimation_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CAnimation_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation_Window::Free()
{
	__super::Free();
}

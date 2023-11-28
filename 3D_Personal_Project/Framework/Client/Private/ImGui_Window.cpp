#include "stdafx.h"
#include "..\Public\ImGui_Window.h"
#include "GameInstance.h"

CImGui_Window::CImGui_Window()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImGui_Window::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	IMGUIWINDESC* pImguiWinDesc = (IMGUIWINDESC*)pArg;

	m_strName = pImguiWinDesc->strName;
	m_window_flags = pImguiWinDesc->window_flags;
	m_vWinSize = pImguiWinDesc->vWinSize;
	m_vBackGroundColor = pImguiWinDesc->vBackGroundColor;

	return S_OK;
}

void CImGui_Window::Begin()
{
	ImGui::PushStyleColor(ImGuiCol_PopupBg, m_vBackGroundColor);

	ImGui::SetNextWindowSize(m_vWinSize, 0);

	ImGui::SetNextWindowBgAlpha(0.5f);

	ImGui::Begin(m_strName.c_str(), 0, m_window_flags);
}

void CImGui_Window::End()
{
	ImGui::PopStyleColor();

	ImGui::End();
}

void CImGui_Window::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}

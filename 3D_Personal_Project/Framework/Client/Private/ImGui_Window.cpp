#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "..\Public\ImGui_Window.h"
#include "GameInstance.h"

typedef struct tagImGui_Window_Desc
	{
		string	strName;	// 창 이름
		ImGuiWindowFlags window_flags;	// 창 옵션
		ImVec2	vWinSize;	// 창 사이즈
		ImVec4 vBackGroundColor = ImVec4(1.f, 1.f, 1.f, 1.f);  // 백 창 색

	}IMGUIWINDESC;

CImGui_Window::CImGui_Window()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImGui_Window::Initialize(void* pArg)
{
	/*if (pArg == nullptr)
		return E_FAIL;

	IMGUIWINDESC* pImguiWinDesc = (IMGUIWINDESC*)pArg;

	m_strName = pImguiWinDesc->strName;
	m_window_flags = pImguiWinDesc->window_flags;
	m_vWinSize = pImguiWinDesc->vWinSize;
	m_vBackGroundColor = pImguiWinDesc->vBackGroundColor;*/
	
	IMGUIWINDESC* paa = new IMGUIWINDESC;
	paa = (IMGUIWINDESC*)pArg;

	//m_pDesc = new IMGUIWINDESC;

	m_pDesc = paa;

	//IMGUIWINDESC* pab = (IMGUIWINDESC*)m_pDesc;

	return S_OK;
}

void CImGui_Window::Begin()
{
	IMGUIWINDESC* pDesc = (IMGUIWINDESC*)m_pDesc;

	ImGui::PushStyleColor(ImGuiCol_PopupBg, pDesc->vBackGroundColor);

	ImGui::SetNextWindowSize(pDesc->vWinSize, 0);

	ImGui::SetNextWindowBgAlpha(0.5f);

	ImGui::Begin(pDesc->strName.c_str(), 0, pDesc->window_flags);
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

#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Public/ImGui_Window.h"
#include "GameInstance.h"

typedef struct tagImGui_Window_Desc
	{
		string	strName;	// â �̸�
		ImGuiWindowFlags window_flags;	// â �ɼ�
		ImVec2	vWinSize;	// â ������
		ImVec4 vBackGroundColor = ImVec4(1.f, 1.f, 1.f, 1.f);  // �� â ��

	}IMGUIWINDESC;

CImGui_Window::CImGui_Window()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImGui_Window::Initialize(void* pArg)
{
	m_pDesc = pArg;

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

	free(m_pDesc);
	Safe_Release(m_pGameInstance);
}

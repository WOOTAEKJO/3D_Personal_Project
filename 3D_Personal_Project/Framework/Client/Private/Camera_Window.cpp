#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Imgui/ImGuiFileDialog-master/ImGuiFileDialog.h"
#include "..\Public\Camera_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"

CCamera_Window::CCamera_Window()
{
}

HRESULT CCamera_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Window::Tick()
{
}

HRESULT CCamera_Window::Render()
{
	CImGui_Window::Begin();

	CImGui_Window::End();

	return S_OK;
}

void CCamera_Window::Set_Variable(void* pArg)
{
}

void CCamera_Window::Terrain_Picked(_float4 vPickPoint)
{
}

void CCamera_Window::Demo_Picked()
{
}

HRESULT CCamera_Window::Save_Data()
{
	//ImGuiFileDialog::Instance()->method_of_your_choice();

	// open Dialog Simple
	if (ImGui::Button("Open File Dialog"))
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");

	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			// action
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	return S_OK;
}

HRESULT CCamera_Window::Load_Data()
{
	return S_OK;
}

CCamera_Window* CCamera_Window::Create(void* pArg)
{
	CCamera_Window* pInstance = new CCamera_Window();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Window::Free()
{
	__super::Free();
}

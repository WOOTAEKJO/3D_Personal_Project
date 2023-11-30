#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "..\Public\Camera_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"

CCamera_Window::CCamera_Window()
{
}

HRESULT CCamera_Window::Initialize(void* pArg)
{
	return S_OK;
}

void CCamera_Window::Tick()
{
}

HRESULT CCamera_Window::Render()
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

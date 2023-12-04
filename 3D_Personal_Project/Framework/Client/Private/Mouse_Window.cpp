#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "..\Public\Mouse_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"

CMouse_Window::CMouse_Window()
{
}

HRESULT CMouse_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMouse_Window::Tick()
{
}

HRESULT CMouse_Window::Render()
{
	CImGui_Window::Begin();

	CImGui_Window::End();

	return S_OK;
}

void CMouse_Window::Set_Variable(void* pArg)
{
}

void CMouse_Window::Terrain_Picked(_float4 vPickPoint)
{
}

void CMouse_Window::Demo_Picked()
{
}

HRESULT CMouse_Window::Save_Data()
{
	return S_OK;
}

HRESULT CMouse_Window::Load_Data()
{
	return S_OK;
}

CMouse_Window* CMouse_Window::Create(void* pArg)
{
	CMouse_Window* pInstance = new CMouse_Window();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CMouse_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMouse_Window::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "..\Public\Win_Test.h"

CWin_Test::CWin_Test()
{
}

HRESULT CWin_Test::Initialize()
{
	//ImGuiWindowFlags windowFlags = 0;


	return S_OK;
}

void CWin_Test::Tick()
{
}

HRESULT CWin_Test::Render()
{
	return S_OK;
}

CWin_Test* CWin_Test::Create()
{
	CWin_Test* pInstance = new CWin_Test();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CWin_Test");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWin_Test::Free()
{
	__super::Free();
}

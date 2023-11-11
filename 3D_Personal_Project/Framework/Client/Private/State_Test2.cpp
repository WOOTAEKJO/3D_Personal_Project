#include "stdafx.h"
#include "..\Public\State_Test2.h"

CState_Test2::CState_Test2()
{
}

HRESULT CState_Test2::Initialize()
{
	return S_OK;
}

void CState_Test2::State_Enter()
{
	int a = 10;
}

void CState_Test2::State_Priority_Tick(_float fTimeDelta)
{
	int a = 10;
}

void CState_Test2::State_Tick(_float fTimeDelta)
{
	int a = 10;
	//SetWindowText(g_hWnd, TEXT("상태2입니다."));
}

void CState_Test2::State_Late_Tick(_float fTimeDelta)
{
	int a = 10;
}

void CState_Test2::State_Exit()
{
	int a = 10;
}

CState_Test2* CState_Test2::Create()
{
	CState_Test2* pInstance = new CState_Test2();

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CState_Test2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CState_Test2::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "..\Public\State_Test1.h"

CState_Test1::CState_Test1()
{
}

HRESULT CState_Test1::Initialize()
{
    return S_OK;
}

void CState_Test1::State_Enter()
{
	int a = 10;
}

void CState_Test1::State_Priority_Tick(_float fTimeDelta)
{
	int a = 10;
}

void CState_Test1::State_Tick(_float fTimeDelta)
{
	int a = 10;
	//SetWindowText(g_hWnd, TEXT("����1�Դϴ�."));
}

void CState_Test1::State_Late_Tick(_float fTimeDelta)
{
	int a = 10;
}

void CState_Test1::State_Exit()
{
	int a = 10;
}

CState_Test1* CState_Test1::Create()
{
	CState_Test1* pInstance = new CState_Test1();

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CState_Test1");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CState_Test1::Free()
{
	__super::Free();
}

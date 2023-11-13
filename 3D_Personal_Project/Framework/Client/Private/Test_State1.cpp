#include "stdafx.h"
#include "..\Public\Test_State1.h"
#include "StateMachine.h"
#include "GameObject_Test.h"

CTest_State1::CTest_State1()
{
}

HRESULT CTest_State1::Initialize(CGameObject* pGameObject)
{
	m_pOwner = dynamic_cast<CGameObject_Test*>(pGameObject);
	if (m_pOwner == nullptr)
		return E_FAIL;

	return S_OK;
}

void CTest_State1::State_Enter()
{
	int a = 0;
}

_uint CTest_State1::State_Priority_Tick(_float fTimeDelta)
{
	int a = 0;

	return m_iStateID;
}

_uint CTest_State1::State_Tick(_float fTimeDelta)
{
	m_pOwner->Action_Test1();

	return m_iStateID;
}

_uint CTest_State1::State_Late_Tick(_float fTimeDelta)
{
	int a = 0;

	if (GetKeyState('P') & 0x8000)
	{
		
		return CGameObject_Test::STATE::STATE2;

	}

	return m_iStateID;
}

void CTest_State1::State_Exit()
{
	int a = 0;
}

CTest_State1* CTest_State1::Create(CGameObject* pGameObject)
{
	CTest_State1* pInstance = new CTest_State1();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CTest_State1");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTest_State1::Free()
{
	__super::Free();
}

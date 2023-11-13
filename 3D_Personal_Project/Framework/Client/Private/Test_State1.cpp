#include "stdafx.h"
#include "..\Public\Test_State1.h"
#include "StateMachine.h"
#include "GameObject_Test.h"

CTest_State1::CTest_State1(CStateMachine* pStateMachine)
	:CState(pStateMachine)
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

void CTest_State1::State_Priority_Tick(_float fTimeDelta)
{
	int a = 0;
}

void CTest_State1::State_Tick(_float fTimeDelta)
{
	m_pOwner->Action_Test1();
}

void CTest_State1::State_Late_Tick(_float fTimeDelta)
{
	int a = 0;

	if (GetKeyState('P') & 0x8000)
	{
		m_pStateMachine->Set_State(CGameObject_Test::STATE::STATE2);
		return;

	}

}

void CTest_State1::State_Exit()
{
	int a = 0;
}

CTest_State1* CTest_State1::Create(CGameObject* pGameObject, CStateMachine* pStateMachine)
{
	CTest_State1* pInstance = new CTest_State1(pStateMachine);

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

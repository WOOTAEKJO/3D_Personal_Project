#include "stdafx.h"
#include "..\Public\Test_State2.h"
#include "StateMachine.h"
#include "GameObject_Test.h"

CTest_State2::CTest_State2(CStateMachine* pStateMachine)
	:CState(pStateMachine)
{
}

HRESULT CTest_State2::Initialize(CGameObject* pGameObject)
{
	m_pOwner = dynamic_cast<CGameObject_Test*>(pGameObject);
	if (m_pOwner == nullptr)
		return E_FAIL;

	return S_OK;
}

void CTest_State2::State_Enter()
{
	int a = 0;
}

void CTest_State2::State_Priority_Tick(_float fTimeDelta)
{
	int a = 0;
}

void CTest_State2::State_Tick(_float fTimeDelta)
{
	m_pOwner->Action_Test2();
}

void CTest_State2::State_Late_Tick(_float fTimeDelta)
{
	int a = 0;

	if (GetKeyState('P') & 0x8000)
	{
		m_pStateMachine->Set_State(CGameObject_Test::STATE::STATE1);
		return;
	}
}

void CTest_State2::State_Exit()
{
	int a = 0;
}

CTest_State2* CTest_State2::Create(CGameObject* pGameObject, CStateMachine* pStateMachine)
{
	CTest_State2* pInstance = new CTest_State2(pStateMachine);

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CTest_State2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTest_State2::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "..\Public\StateCharge_Test.h"
#include "StateMachine.h"
#include "GameObject_Test.h"

CStateCharge_Test::CStateCharge_Test(CGameObject* pOwner, CStateMachine* pStateMachine)
	:CStateCharge(pOwner, pStateMachine)
{
}

HRESULT CStateCharge_Test::Initialize()
{
	/*if (FAILED(Add_State1()))
		return E_FAIL;

	if (FAILED(Add_State2()))
		return E_FAIL;


	if (FAILED(m_pStateMachine->Init_State(STATE::STATE1)))
		return E_FAIL;*/
	
	return S_OK;
}

HRESULT CStateCharge_Test::Add_State1()
{
	if (m_pOwner == nullptr ||
		m_pStateMachine == nullptr)
		return E_FAIL;

	CGameObject_Test* pOwner = dynamic_cast<CGameObject_Test*>(m_pOwner);

#pragma region 상태 만들기

	if (FAILED(m_pStateMachine->Add_State(STATE::STATE1)))
		return E_FAIL;

#pragma endregion

#pragma region 행동 설정

	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE1, CStateMachine::STATE::ENTER, [&]() {
		pOwner->Action_Test1();
		})))
		return E_FAIL;
	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE1, CStateMachine::STATE::PRIORITY_TICK, [&]() {
		pOwner->Action_Test1();
		})))
		return E_FAIL;
	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE1, CStateMachine::STATE::TICK, [&]() {
		pOwner->Action_Test1();
		})))
		return E_FAIL;
	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE1, CStateMachine::STATE::LATE_TICK, [&]() {
		pOwner->Action_Test1();
		})))
		return E_FAIL;
	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE1, CStateMachine::STATE::EXIT, [&]() {
		pOwner->Action_Test1();
		})))
		return E_FAIL;

#pragma endregion	

#pragma region 전이 설정

	if(FAILED(m_pStateMachine->Add_Transition(STATE::STATE1, STATE::STATE2, [&]()->bool {
		return pOwner->Transition_Test1();
		})))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CStateCharge_Test::Add_State2()
{
	if (m_pOwner == nullptr ||
		m_pStateMachine == nullptr)
		return E_FAIL;

	CGameObject_Test* pOwner = dynamic_cast<CGameObject_Test*>(m_pOwner);

#pragma region 상태 만들기

	if (FAILED(m_pStateMachine->Add_State(STATE::STATE2)))
		return E_FAIL;

#pragma endregion

#pragma region 행동 설정

	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE2, CStateMachine::STATE::ENTER, [&]() {
		pOwner->Action_Test2();
		})))
		return E_FAIL;
	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE2, CStateMachine::STATE::PRIORITY_TICK, [&]() {
		pOwner->Action_Test2();
		})))
		return E_FAIL;
	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE2, CStateMachine::STATE::TICK, [&]() {
		pOwner->Action_Test2();
		})))
		return E_FAIL;
	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE2, CStateMachine::STATE::LATE_TICK, [&]() {
		pOwner->Action_Test2();
		})))
		return E_FAIL;
	if (FAILED(m_pStateMachine->Add_Action(STATE::STATE2, CStateMachine::STATE::EXIT, [&]() {
		pOwner->Action_Test2();
		})))
		return E_FAIL;

#pragma endregion	

#pragma region 전이 설정

	if (FAILED(m_pStateMachine->Add_Transition(STATE::STATE2, STATE::STATE1, [&]()->bool {
		return pOwner->Transition_Test2();
		})))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

CStateCharge_Test* CStateCharge_Test::Create(CGameObject* pOwner, CStateMachine* pStateMachine)
{
	CStateCharge_Test* pInstance = new CStateCharge_Test(pOwner, pStateMachine);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CStateCharge_Test");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStateCharge_Test::Free()
{
	__super::Free();
}

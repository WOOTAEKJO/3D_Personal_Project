#include "..\Public\State.h"
#include "Action.h"

CState::CState()
{
	m_vecActions.reserve(5);
}

HRESULT CState::Initialize()
{
	for (size_t i = 0; i < CState::STATE::STATE_END; i++) {
		CAction* pAction = CAction::Create();
		if (pAction == nullptr)
			return E_FAIL;
		m_vecActions.push_back(pAction);
	}

    return S_OK;
}

void CState::State_Enter()
{
	auto& iter = m_vecActions[CState::STATE::ENTER];

	if (iter != nullptr)
		iter->Act();
}

void CState::State_Priority_Tick(_float fTimeDelta)
{
	auto& iter = m_vecActions[CState::STATE::PRIORITY_TICK];

	if (iter != nullptr)
		iter->Act();

}

void CState::State_Tick(_float fTimeDelta)
{
	auto& iter = m_vecActions[CState::STATE::TICK];

	if (iter != nullptr)
		iter->Act();
}

void CState::State_Late_Tick(_float fTimeDelta)
{
	auto& iter = m_vecActions[CState::STATE::LATE_TICK];

	if (iter != nullptr)
		iter->Act();
}

void CState::State_Exit()
{
	auto& iter = m_vecActions[CState::STATE::EXIT];

	if (iter != nullptr)
		iter->Act();
}

HRESULT CState::Add_Action(STATE eTickType, function<void()> pFunction)
{
	if (pFunction == nullptr||
		m_vecActions[eTickType] == nullptr)
		return E_FAIL;

	if (FAILED(m_vecActions[eTickType]->Add_Action(pFunction)))
		return E_FAIL;
		
	return S_OK;
}

CState* CState::Create()
{
	CState* pInstance = new CState();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState::Free()
{
	__super::Free();

	for (auto& iter : m_vecActions)
		Safe_Release(iter);
	m_vecActions.clear();
}

#include "..\Public\StateMachine.h"
#include "State.h"
#include "Transition.h"
#include "Action.h"

CStateMachine::CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CStateMachine::CStateMachine(const CStateMachine& rhs)
	:CComponent(rhs)
{
}

HRESULT CStateMachine::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CStateMachine::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CStateMachine::Add_State(const wstring& strStateTag, CState* pState)
{
	if (pState == nullptr)
		return E_FAIL;

	CState* pSState = Find_State(strStateTag);

	if (pSState != nullptr)
		return E_FAIL;

	m_mapState.emplace(strStateTag, pState);

	return S_OK;
}

HRESULT CStateMachine::Add_Transition(const wstring& strStateTag, function<bool()> pFunction, const wstring& strResultStateTag)
{
	if (pFunction == nullptr)
		return E_FAIL;

	CTransition* pTransition = Find_Transition(strStateTag);

	if (pTransition == nullptr)
	{
		pTransition = CTransition::Create();
		if (pTransition == nullptr)
			return E_FAIL;

		if (FAILED(pTransition->Add_Transition(pFunction, strResultStateTag)))
			return E_FAIL;

		m_mapTransition.emplace(strStateTag, pTransition);
	}
	else {
		if (FAILED(pTransition->Add_Transition(pFunction, strResultStateTag)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStateMachine::Add_Action(const wstring& strStateTag, TICKSTATE eTickType, function<void()> pFunction)
{
	if (pFunction == nullptr)
		return E_FAIL;

	CAction* pAction = Find_Action(strStateTag, eTickType);

	if (pAction == nullptr)
	{
		pAction = CAction::Create();
		if (pAction == nullptr)
			return E_FAIL;

		if (FAILED(pAction->Add_Action(pFunction)))
			return E_FAIL;

		m_mapAction[eTickType].emplace(strStateTag, pAction);
	}
	else {
		if (FAILED(pAction->Add_Action(pFunction)))
			return E_FAIL;
	}

	return S_OK;
}

void CStateMachine::Priority_Tick(_float fTimeDelta)
{
	if (m_pCurrentState == nullptr)
		return;

	if (!Is_Change_State()) {
		m_pCurrentState->State_Priority_Tick(fTimeDelta);
	
		CAction* pAction = Find_Action(m_strCurrentStateTag, TICKSTATE::PRIORITY_TICK);
		if (pAction == nullptr)
			return;
		pAction->Tick(fTimeDelta);
	}
}

void CStateMachine::Tick(_float fTimeDelta)
{
	if (m_pCurrentState == nullptr)
		return;

	if (!Is_Change_State()) {
		m_pCurrentState->State_Tick(fTimeDelta);

		CAction* pAction = Find_Action(m_strCurrentStateTag, TICKSTATE::TICK);
		if (pAction == nullptr)
			return;
		pAction->Tick(fTimeDelta);
	}
}

void CStateMachine::Late_Tick(_float fTimeDelta)
{
	if (m_pCurrentState == nullptr)
		return;

	if (!Is_Change_State()) {
		m_pCurrentState->State_Late_Tick(fTimeDelta);

		CAction* pAction = Find_Action(m_strCurrentStateTag, TICKSTATE::LATE_TICK);
		if (pAction == nullptr)
			return;
		pAction->Tick(fTimeDelta);
	}
}

HRESULT CStateMachine::Init_State(const wstring& strStateTag)
{
	CState* pState = Find_State(strStateTag);
	if (pState == nullptr)
		return E_FAIL;

	m_pCurrentState = pState;
	m_strCurrentStateTag = strStateTag;
	m_pCurrentState->State_Enter();

	return S_OK;
}

HRESULT CStateMachine::Set_State(const wstring& strStateTag)
{
	if (m_pCurrentState == nullptr)
		return E_FAIL;

	m_pCurrentState->State_Exit();

	CState* pState = Find_State(strStateTag);
	if (pState == nullptr)
		return E_FAIL;

	m_pCurrentState = pState;
	m_strCurrentStateTag = strStateTag;
	m_pCurrentState->State_Enter();

	return S_OK;
}

bool CStateMachine::Is_Change_State()
{
	wstring	strResultStateTag = TEXT("");

	CTransition* pTransition = Find_Transition(m_strCurrentStateTag);
	if (pTransition == nullptr)
		return false;

	if (pTransition->Is_Transition(&strResultStateTag)) {
		Set_State(strResultStateTag);
		return true;
	}
	
	return false;
}

CState* CStateMachine::Find_State(const wstring& strStateTag)
{
	auto& iter = m_mapState.find(strStateTag);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

CTransition* CStateMachine::Find_Transition(const wstring& strStateTag)
{
	auto& iter = m_mapTransition.find(strStateTag);

	if (iter == m_mapTransition.end())
		return nullptr;

	return iter->second;
}

CAction* CStateMachine::Find_Action(const wstring& strStateTag, TICKSTATE eTickType)
{
	auto& iter = m_mapAction[eTickType].find(strStateTag);

	if (iter == m_mapAction[eTickType].end())
		return nullptr;

	return iter->second;
}

CStateMachine* CStateMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStateMachine* pInstance = new CStateMachine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CStateMachine::Clone(void* pArg)
{
	CStateMachine* pInstance = new CStateMachine(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Created : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateMachine::Free()
{
	__super::Free();
}


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

HRESULT CStateMachine::Add_State(const _uint& iStateID)
{
	CState* pState = Find_State(iStateID);

	if (pState != nullptr)
		return E_FAIL;

	pState = CState::Create();
	if (pState == nullptr)
		return E_FAIL;

	m_mapState.emplace(iStateID, pState);

	return S_OK;
}

HRESULT CStateMachine::Add_Transition(const _uint& iStateID, const _uint& iResultStateID, function<bool()> pFunction)
{
	if (pFunction == nullptr)
		return E_FAIL;

	CState* pState = Find_State(iStateID);
	if (pState == nullptr)
		return E_FAIL;

	if (FAILED(pState->Add_Transition(iResultStateID, pFunction)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CStateMachine::Add_Action(const _uint& iStateID, STATE eStateType, function<void()> pFunction)
{
	if (pFunction == nullptr)
		return E_FAIL;

	CState* pState = Find_State(iStateID);
	if (pState == nullptr)
		return E_FAIL;

	if (FAILED(pState->Add_Action((CState::STATE)eStateType, pFunction)))
		return E_FAIL;

	return S_OK;
}

void CStateMachine::Priority_Tick(_float fTimeDelta)
{
	if (m_pCurrentState == nullptr)
		return;

	if (!Is_Change_State()) {
		m_pCurrentState->State_Priority_Tick(fTimeDelta);
	}
}

void CStateMachine::Tick(_float fTimeDelta)
{
	if (m_pCurrentState == nullptr)
		return;

	if (!Is_Change_State()) {
		m_pCurrentState->State_Tick(fTimeDelta);
	}
}

void CStateMachine::Late_Tick(_float fTimeDelta)
{
	if (m_pCurrentState == nullptr)
		return;

	if (!Is_Change_State()) {
		m_pCurrentState->State_Late_Tick(fTimeDelta);
	}
}

HRESULT CStateMachine::Init_State(const _uint& iStateID)
{
	CState* pState = Find_State(iStateID);
	if (pState == nullptr)
		return E_FAIL;

	m_pCurrentState = pState;
	m_iCurrentStateID = iStateID;
	m_pCurrentState->State_Enter();

	return S_OK;
}

HRESULT CStateMachine::Set_State(const _uint& iStateID)
{
	if (m_pCurrentState == nullptr)
		return E_FAIL;

	m_pCurrentState->State_Exit();

	CState* pState = Find_State(iStateID);
	if (pState == nullptr)
		return E_FAIL;

	m_pCurrentState = pState;
	m_iCurrentStateID = iStateID;
	m_pCurrentState->State_Enter();

	return S_OK;
}

bool CStateMachine::Is_Change_State()
{
	_uint		iResultStateID = 0;

	if (m_pCurrentState == nullptr)
		return false;

	if (m_pCurrentState->Is_Transition(&iResultStateID)) {
		Set_State(iResultStateID);
		return true;
	}
	
	return false;
}

CState* CStateMachine::Find_State(const _uint& iStateID)
{
	auto& iter = m_mapState.find(iStateID);

	if (iter == m_mapState.end())
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

	for (auto& iter : m_mapState)
		Safe_Release(iter.second);
	m_mapState.clear();
}


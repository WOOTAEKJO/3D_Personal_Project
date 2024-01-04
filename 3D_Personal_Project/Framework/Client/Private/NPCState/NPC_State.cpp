#include "stdafx.h"
#include "..\Public\NPCState\NPC_State.h"
#include "StateMachine.h"


CNPC_State::CNPC_State()
{
}

HRESULT CNPC_State::Initialize(CGameObject* pGameObject)
{
	m_pOwner = dynamic_cast<CNPC*>(pGameObject);
	if (m_pOwner == nullptr)
		return E_FAIL;

	m_pOwnerModel = m_pOwner->Get_Component<CModel>();
	Safe_AddRef(m_pOwnerModel);
	m_pOnwerTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pOnwerTransform);

	return S_OK;
}

void CNPC_State::State_Enter()
{

}

_uint CNPC_State::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CNPC_State::State_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CNPC_State::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNPC_State::State_Exit()
{
}

void CNPC_State::Free()
{
	__super::Free();
}

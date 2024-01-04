#include "stdafx.h"
#include "..\Public\NPCState\NPC_IDLE.h"

#include "Player.h"
#include "Crow.h"

CNPC_IDLE::CNPC_IDLE()
{
}

HRESULT CNPC_IDLE::Initialize(CGameObject* pGameObject)
{
	if (FAILED(__super::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNPC_IDLE::State_Enter()
{
	m_pOwner->Set_TypeAnimIndex(CNPC::STATE::IDLE);
	dynamic_cast<CCrow*>(m_pOwner)->Set_IDLE_Pos();
}

_uint CNPC_IDLE::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CNPC_IDLE::State_Tick(_float fTimeDelta)
{
	if (dynamic_cast<CCrow*>(m_pOwner)->Attack_Input())
		return CNPC::STATE::ATTACK;

	if (!m_pOwner->Is_Target_State(CPlayer::STATE::IDLE))
	{
		return CNPC::STATE::FOLLOW;
	}

	m_pOwner->PlayerLook();

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNPC_IDLE::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNPC_IDLE::State_Exit()
{
}

CNPC_IDLE* CNPC_IDLE::Create(CGameObject* pGameObject)
{
	CNPC_IDLE* pInstance = new CNPC_IDLE();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNPC_IDLE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNPC_IDLE::Free()
{
	__super::Free();
}

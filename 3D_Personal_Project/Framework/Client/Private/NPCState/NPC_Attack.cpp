#include "stdafx.h"
#include "..\Public\NPCState\NPC_Attack.h"

#include "Player.h"
#include "Crow.h"

CNPC_Attack::CNPC_Attack()
{
}

HRESULT CNPC_Attack::Initialize(CGameObject* pGameObject)
{
	if (FAILED(__super::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNPC_Attack::State_Enter()
{
	m_pOwner->Set_TypeAnimIndex(CNPC::STATE::ATTACK);

	m_bFind = dynamic_cast<CCrow*>(m_pOwner)->Find_Range_Monster(5.f);
	m_pOwner->Trans_Attack(true);
}

_uint CNPC_Attack::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CNPC_Attack::State_Tick(_float fTimeDelta)
{
	if(!m_bFind)
		return CNPC::STATE::IDLE;

	if (m_pOwner->Turn(fTimeDelta * 10.f))
	{
		m_pOwner->Target_Follow(fTimeDelta * 20.f);
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNPC_Attack::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwner->Get_NPCType() == CNPC::NPC_TYPE::CROW)
	{
		/*if (m_pOwner->Is_Target_Range(0.1f))
			return CNPC::STATE::IDLE;*/
		_bool b = dynamic_cast<CCrow*>(m_pOwner)->Is_Col();

		if (b)
			return CNPC::STATE::IDLE;

	}

	return m_iStateID;
}

void CNPC_Attack::State_Exit()
{
	m_pOwner->Trans_Attack(false);
	m_bFind = false;
}

CNPC_Attack* CNPC_Attack::Create(CGameObject* pGameObject)
{
	CNPC_Attack* pInstance = new CNPC_Attack();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNPC_Attack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNPC_Attack::Free()
{
	__super::Free();
}

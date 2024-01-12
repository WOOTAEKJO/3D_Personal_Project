#include "stdafx.h"
#include "..\Public\MonsterState\NorMonster_Move.h"
#include "StateMachine.h"


CNorMonster_Move::CNorMonster_Move()
{
}

HRESULT CNorMonster_Move::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNorMonster_Move::State_Enter()
{
	
	m_pOwnerModel->Set_AnimationIndex(10);
}

_uint CNorMonster_Move::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CNorMonster_Move::State_Tick(_float fTimeDelta)
{
	
	if (!m_pOwner->Is_Target_Range(7.f * 0.3f))
	{
		return CMonster::STATE::IDLE;
	}

	if (m_pOwner->Open_Status_Desc()->bHited)
		return CMonster::STATE::HITED;

	if (m_pOwner->Turn(fTimeDelta))
	{
		if (m_pOwner->Is_Target_Range(2.f * 0.16f))
		{
			return CMonster::STATE::ATTACK;
		}

		Translate(CTransform::STATE::STATE_LOOK, 0.3f, fTimeDelta);
	}
		

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNorMonster_Move::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNorMonster_Move::State_Exit()
{
}

CNorMonster_Move* CNorMonster_Move::Create(CGameObject* pGameObject)
{
	CNorMonster_Move* pInstance = new CNorMonster_Move();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNorMonster_Move");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNorMonster_Move::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MonsterState\NorMonster_Appear.h"
#include "StateMachine.h"


CNorMonster_Appear::CNorMonster_Appear()
{
}

HRESULT CNorMonster_Appear::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNorMonster_Appear::State_Enter()
{
	if (m_pOwner->Get_MonsterType() == CMonster::MONSTER_TYPE::SPOOKETON)
		m_pOwnerModel->Set_AnimationIndex(9);
	else if (m_pOwner->Get_MonsterType() == CMonster::MONSTER_TYPE::SKULLCROSSBOW)
		m_pOwnerModel->Set_AnimationIndex(10);
}

_uint CNorMonster_Appear::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CNorMonster_Appear::State_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CMonster::STATE::IDLE;
	}


	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CNorMonster_Appear::State_Late_Tick(_float fTimeDelta)
{
	
	return m_iStateID;
}

void CNorMonster_Appear::State_Exit()
{
}

CNorMonster_Appear* CNorMonster_Appear::Create(CGameObject* pGameObject)
{
	CNorMonster_Appear* pInstance = new CNorMonster_Appear();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNorMonster_Appear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNorMonster_Appear::Free()
{
	__super::Free();

}

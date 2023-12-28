#include "stdafx.h"
#include "..\Public\MonsterState\NorMonster_Attack.h"
#include "StateMachine.h"


CNorMonster_Attack::CNorMonster_Attack()
{
}

HRESULT CNorMonster_Attack::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNorMonster_Attack::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(5);
}

_uint CNorMonster_Attack::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CNorMonster_Attack::State_Tick(_float fTimeDelta)
{		

	Is_Attack_Time(m_pOwner->Get_WeaponCollider(),fTimeDelta,0.5f);

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CNorMonster_Attack::State_Late_Tick(_float fTimeDelta)
{

	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CMonster::STATE::DELAY;
	}

	return m_iStateID;
}

void CNorMonster_Attack::State_Exit()
{
	Reset_Attack_Time(m_pOwner->Get_WeaponCollider());
}

CNorMonster_Attack* CNorMonster_Attack::Create(CGameObject* pGameObject)
{
	CNorMonster_Attack* pInstance = new CNorMonster_Attack();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNorMonster_Attack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNorMonster_Attack::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MonsterState\NorMonster_Hited.h"
#include "StateMachine.h"


CNorMonster_Hited::CNorMonster_Hited()
{
}

HRESULT CNorMonster_Hited::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNorMonster_Hited::State_Enter()
{		
	if (m_pOwner->Get_MonsterType() == CMonster::MONSTER_TYPE::SPOOKETON)
		m_pOwnerModel->Set_AnimationIndex(7);
	else if (m_pOwner->Get_MonsterType() == CMonster::MONSTER_TYPE::SKULLCROSSBOW)
		m_pOwnerModel->Set_AnimationIndex(2);
}

_uint CNorMonster_Hited::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CNorMonster_Hited::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CNorMonster_Hited::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CMonster::STATE::IDLE;
	}

	return m_iStateID;
}

void CNorMonster_Hited::State_Exit()
{
	m_pOwner->Open_Status_Desc()->bHited = false;
}

CNorMonster_Hited* CNorMonster_Hited::Create(CGameObject* pGameObject)
{
	CNorMonster_Hited* pInstance = new CNorMonster_Hited();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNorMonster_Hited");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNorMonster_Hited::Free()
{
	__super::Free();

}

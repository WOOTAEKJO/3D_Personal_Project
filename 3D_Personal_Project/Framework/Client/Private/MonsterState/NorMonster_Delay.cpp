#include "stdafx.h"
#include "..\Public\MonsterState\NorMonster_Delay.h"
#include "StateMachine.h"


CNorMonster_Delay::CNorMonster_Delay()
{
}

HRESULT CNorMonster_Delay::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNorMonster_Delay::State_Enter()
{
	if (m_pOwner->Get_MonsterType() == CMonster::MONSTER_TYPE::SPOOKETON)
		m_pOwnerModel->Set_AnimationIndex(8);
	else if (m_pOwner->Get_MonsterType() == CMonster::MONSTER_TYPE::SKULLCROSSBOW)
		m_pOwnerModel->Set_AnimationIndex(3);
}

_uint CNorMonster_Delay::State_Priority_Tick(_float fTimeDelta)
{
	if (m_pOwner->Open_Status_Desc()->bHited)
		return CMonster::STATE::HITED;

	return m_iStateID;
}

_uint CNorMonster_Delay::State_Tick(_float fTimeDelta)
{

	m_fTime += fTimeDelta;

	if (m_fTime > 1.f)
		return CMonster::STATE::IDLE;

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNorMonster_Delay::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNorMonster_Delay::State_Exit()
{
	m_fTime = 0.f;
}

CNorMonster_Delay* CNorMonster_Delay::Create(CGameObject* pGameObject)
{
	CNorMonster_Delay* pInstance = new CNorMonster_Delay();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNorMonster_Delay");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNorMonster_Delay::Free()
{
	__super::Free();

}

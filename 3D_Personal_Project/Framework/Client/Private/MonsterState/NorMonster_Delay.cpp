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
	m_pOwnerModel->Set_AnimationIndex(8);
}

_uint CNorMonster_Delay::State_Priority_Tick(_float fTimeDelta)
{
	m_pOwner->TargetLook();

	return m_iStateID;
}

_uint CNorMonster_Delay::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNorMonster_Delay::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNorMonster_Delay::State_Exit()
{
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

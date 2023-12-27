#include "stdafx.h"
#include "..\Public\MonsterState\NorMonster_IDLE.h"
#include "StateMachine.h"


CNorMonster_IDLE::CNorMonster_IDLE()
{
}

HRESULT CNorMonster_IDLE::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNorMonster_IDLE::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(8);
}

_uint CNorMonster_IDLE::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CNorMonster_IDLE::State_Tick(_float fTimeDelta)
{

	if (m_pOwner->Is_Target_Range(7.f))
	{
		return CMonster::STATE::MOVE;
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNorMonster_IDLE::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNorMonster_IDLE::State_Exit()
{
}

CNorMonster_IDLE* CNorMonster_IDLE::Create(CGameObject* pGameObject)
{
	CNorMonster_IDLE* pInstance = new CNorMonster_IDLE();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNorMonster_IDLE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNorMonster_IDLE::Free()
{
	__super::Free();

}

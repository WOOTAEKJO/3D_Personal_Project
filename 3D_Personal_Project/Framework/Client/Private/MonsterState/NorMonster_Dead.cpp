#include "stdafx.h"
#include "..\Public\MonsterState\NorMonster_Dead.h"
#include "StateMachine.h"


CNorMonster_Dead::CNorMonster_Dead()
{
}

HRESULT CNorMonster_Dead::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNorMonster_Dead::State_Enter()
{
	//m_pOwnerModel->Set_AnimationIndex(8);
}

_uint CNorMonster_Dead::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CNorMonster_Dead::State_Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > 2.f)
		m_pOwner->Set_Dead();
	
	//m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNorMonster_Dead::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNorMonster_Dead::State_Exit()
{
}

CNorMonster_Dead* CNorMonster_Dead::Create(CGameObject* pGameObject)
{
	CNorMonster_Dead* pInstance = new CNorMonster_Dead();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNorMonster_Dead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNorMonster_Dead::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Summon_Bomb.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Summon_Bomb::CPhantom_Summon_Bomb()
{
}

HRESULT CPhantom_Summon_Bomb::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Summon_Bomb::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Summon_Bomb::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Summon_Bomb::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Summon_Bomb::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Summon_Bomb::State_Exit()
{
}

CPhantom_Summon_Bomb* CPhantom_Summon_Bomb::Create(CGameObject* pGameObject)
{
	CPhantom_Summon_Bomb* pInstance = new CPhantom_Summon_Bomb();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Summon_Bomb");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Summon_Bomb::Free()
{
	__super::Free();

}

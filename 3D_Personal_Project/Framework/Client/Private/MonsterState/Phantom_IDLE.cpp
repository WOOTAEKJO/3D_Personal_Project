#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_IDLE.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_IDLE::CPhantom_IDLE()
{
}

HRESULT CPhantom_IDLE::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_IDLE::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::IDLE);

}

_uint CPhantom_IDLE::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_IDLE::State_Tick(_float fTimeDelta)
{
	
	if (!m_pOwner->Open_Status_Desc()->bTalk)
	{
		//return CPhantom::STATE::MARTEAU;
		//return CPhantom::STATE::CHASSE;
		//return CPhantom::STATE::LASER;
		//return CPhantom::STATE::VANISH;
		return CPhantom::STATE::APPEAR;
		//return CPhantom::STATE::DEAD;
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_IDLE::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_IDLE::State_Exit()
{
}

CPhantom_IDLE* CPhantom_IDLE::Create(CGameObject* pGameObject)
{
	CPhantom_IDLE* pInstance = new CPhantom_IDLE();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_IDLE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_IDLE::Free()
{
	__super::Free();

}

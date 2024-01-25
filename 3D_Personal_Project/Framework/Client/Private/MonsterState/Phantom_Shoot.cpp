#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Shoot.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Shoot::CPhantom_Shoot()
{
}

HRESULT CPhantom_Shoot::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Shoot::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::SHOOT);
	m_pOwnerCollider->Set_UseCol(true);
}

_uint CPhantom_Shoot::State_Priority_Tick(_float fTimeDelta)
{
	if(m_pOwnerCollider->Get_Collision())
		return CPhantom::STATE::HIT;

	if (dynamic_cast<CPhantom*>(m_pOwner)->Is_Bomb_Failed())
	{
		dynamic_cast<CPhantom*>(m_pOwner)->Delete_Bomb();
		return CPhantom::STATE::SUMMON_BOMB;
	}

	return m_iStateID;
}

_uint CPhantom_Shoot::State_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CPhantom::STATE::SHOOT, 36.f))
	{
		if (m_bAttack)
		{
			dynamic_cast<CPhantom*>(m_pOwner)->Create_TargetBullet();
			m_bAttack = false;
		}
			
	}

	m_pOwner->TargetLook();
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Shoot::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::SHOOT);
		m_bAttack = true;
	}

	return m_iStateID;
}

void CPhantom_Shoot::State_Exit()
{
	m_pOwnerCollider->Set_UseCol(false);
}

CPhantom_Shoot* CPhantom_Shoot::Create(CGameObject* pGameObject)
{
	CPhantom_Shoot* pInstance = new CPhantom_Shoot();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Shoot");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Shoot::Free()
{
	__super::Free();

}

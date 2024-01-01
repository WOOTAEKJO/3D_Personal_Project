#include "stdafx.h"
#include "..\Public\PlayerState\Player_Spear_AirAttack.h"

#include "GameInstance.h"

CPlayer_Spear_AirAttack::CPlayer_Spear_AirAttack()
{
}

HRESULT CPlayer_Spear_AirAttack::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Spear_AirAttack::State_Enter()
{
	//m_pOwnerModel->Set_AnimationIndex(76);
	m_pOwner->Animation_By_Type(CPlayer::STATE::AIR_ATTACK);
}

_uint CPlayer_Spear_AirAttack::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CPlayer_Spear_AirAttack::State_Tick(_float fTimeDelta)
{
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	/*if (m_pOwnerModel->Is_Animation_Finished())
		return CPlayer::STATE::IDLE;*/

	return m_iStateID;
}

_uint CPlayer_Spear_AirAttack::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwner->Open_Physics_Desc()->bFall && !m_pOwner->Open_Physics_Desc()->bLanding
		&& m_pOnwerRigidBody->Is_Land())
	{
		m_pOnwerRigidBody->Land();
	
		if (m_pOwnerModel->Is_Animation_Finished()) {
			m_pOwner->Open_Physics_Desc()->bLanding = true;
			m_pOwner->Open_Physics_Desc()->bDoubleJump = false;
			m_pOwner->Open_Physics_Desc()->bFall = false;
			m_pOwner->Open_Physics_Desc()->bJump = false;
			m_pOnwerTransform->Set_Ground(true);

			return CPlayer::STATE::IDLE;
		}
		
	}

	if (!m_pOwner->Open_Physics_Desc()->bFall)
	{
		if (m_pOnwerRigidBody->Is_Power_Zero(CRigidBody::TYPE::TYPE_VELOCITY)) {

			m_pOwner->Open_Physics_Desc()->bFall = true;
			m_pOnwerRigidBody->Set_GravityPower(m_pOwner->Open_Physics_Desc()->fFallGravity * 3.f);
		}
	}

	return m_iStateID;
}

void CPlayer_Spear_AirAttack::State_Exit()
{
	m_pOwner->Open_Physics_Desc()->bGround = true;
	m_pOwner->Open_Physics_Desc()->bLanding = false;
}

CPlayer_Spear_AirAttack* CPlayer_Spear_AirAttack::Create(CGameObject* pGameObject)
{
	CPlayer_Spear_AirAttack* pInstance = new CPlayer_Spear_AirAttack();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Spear_AirAttack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Spear_AirAttack::Free()
{
	__super::Free();
}

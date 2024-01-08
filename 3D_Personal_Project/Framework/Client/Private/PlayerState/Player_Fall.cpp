#include "stdafx.h"
#include "..\Public\PlayerState\Player_Fall.h"

#include "GameInstance.h"

CPlayer_Fall::CPlayer_Fall()
{
}

HRESULT CPlayer_Fall::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Fall::State_Enter()
{
	if (m_pOwner->Open_Physics_Desc()->bDoubleJump)
		m_pOwner->Animation_By_Type(CPlayer::STATE::FALL);

	if (Falling())
	{
		m_pOwner->Open_Physics_Desc()->bGround = false;
		m_pOwner->Open_Physics_Desc()->bJump = true;
		m_pOnwerTransform->Set_Ground(false);
		m_pOnwerRigidBody->Set_GravityCheck(true);
	}
	
	m_pOwner->Open_Physics_Desc()->bFall = true;
	m_pOnwerRigidBody->Set_GravityPower(m_pOwner->Open_Physics_Desc()->fFallGravity);
}

_uint CPlayer_Fall::State_Priority_Tick(_float fTimeDelta)
{
	if (m_pOwner->Open_Physics_Desc()->bFall && !m_pOwner->Open_Physics_Desc()->bLanding
 		&& m_pOnwerRigidBody->Is_Land())
	{
		if (m_pOwner->Open_Physics_Desc()->bDoubleJump) {
			Land();
			return CPlayer::STATE::LAND;
		}
		else {
			Land();
			return CPlayer::STATE::IDLE;
		}
	}

	return m_iStateID;
}

_uint CPlayer_Fall::State_Tick(_float fTimeDelta)
{
	Move(fTimeDelta);

	m_pOwner->Get_BodyModel()->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPlayer_Fall::State_Late_Tick(_float fTimeDelta)
{
	

	if (!m_pOwner->Open_Physics_Desc()->bDoubleJump)
	{
		if (m_pOnwerController->Key_Down(CPlayer::KEY_STATE::KEY_JUMP))
		{
			//m_pOwner->Animation_By_Type(CPlayer::STATE::DOUBLEJUMP);
			m_pOwner->Open_Physics_Desc()->bFall = false;
			m_pOwner->Open_Physics_Desc()->bDoubleJump = true;
			//Jump();
			return CPlayer::STATE::JUMP;
		}
	}

	/*if (m_pOwner->Open_Physics_Desc()->bDoubleJump)
	{
		if (m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
		{
			return CPlayer::STATE::AIR_ATTACK;
		}
	}*/

	return m_iStateID;
}

void CPlayer_Fall::State_Exit()
{
	if (m_pOwner->Open_Physics_Desc()->bLanding)
	{
		m_pOwner->Open_Physics_Desc()->bGround = true;
		m_pOwner->Open_Physics_Desc()->bLanding = false;
	}

	m_pOnwerNavigation->Set_NaviFall(false);
}

void CPlayer_Fall::Land()
{
	m_pOnwerRigidBody->Land();

	m_pOwner->Open_Physics_Desc()->bDoubleJump = false;
	m_pOwner->Open_Physics_Desc()->bFall = false;
	m_pOwner->Open_Physics_Desc()->bJump = false;
	m_pOwner->Open_Physics_Desc()->bLanding = true;
	m_pOnwerTransform->Set_Ground(true);
}

void CPlayer_Fall::Jump()
{
	m_pOnwerRigidBody->Jump(m_pOwner->Open_Physics_Desc()->fJumpPower, m_pOwner->Open_Physics_Desc()->fJumpGravity);
}

CPlayer_Fall* CPlayer_Fall::Create(CGameObject* pGameObject)
{
	CPlayer_Fall* pInstance = new CPlayer_Fall();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Fall");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Fall::Free()
{
	__super::Free();
}

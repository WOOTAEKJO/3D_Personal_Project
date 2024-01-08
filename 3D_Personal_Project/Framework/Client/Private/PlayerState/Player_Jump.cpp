#include "stdafx.h"
#include "..\Public\PlayerState\Player_Jump.h"

#include "GameInstance.h"

CPlayer_Jump::CPlayer_Jump()
{
}

HRESULT CPlayer_Jump::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Jump::State_Enter()
{
	//m_pOwnerModel->Set_AnimationIndex(84);
	if(m_pOwner->Open_Physics_Desc()->bDoubleJump)
		m_pOwner->Animation_By_Type(CPlayer::STATE::DOUBLEJUMP);
	else
		m_pOwner->Animation_By_Type(CPlayer::STATE::JUMP);

	m_pOwner->Open_Physics_Desc()->bGround = false;
	m_pOwner->Open_Physics_Desc()->bJump = true;
	m_pOnwerTransform->Set_Ground(false);
	Jump();
}

_uint CPlayer_Jump::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CPlayer_Jump::State_Tick(_float fTimeDelta)
{
	Move(fTimeDelta);

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPlayer_Jump::State_Late_Tick(_float fTimeDelta)
{
	/*if (m_pOwner->Open_Physics_Desc()->bFall && !m_pOwner->Open_Physics_Desc()->bLanding
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
	}*/

	if (!m_pOwner->Open_Physics_Desc()->bFall)
	{
		if (Is_Fall())
			return CPlayer::STATE::FALL;
	}

	if (!m_pOwner->Open_Physics_Desc()->bDoubleJump)
	{
		if (m_pOnwerController->Key_Down(CPlayer::KEY_STATE::KEY_JUMP))
		{
			m_pOwner->Animation_By_Type(CPlayer::STATE::DOUBLEJUMP);
			m_pOwner->Open_Physics_Desc()->bFall = false;
			m_pOwner->Open_Physics_Desc()->bDoubleJump = true;
			Jump();
		}
	}

	if (m_pOwner->Open_Physics_Desc()->bDoubleJump)
	{
		if (m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
		{
			return CPlayer::STATE::AIR_ATTACK;
		}
	}

	return m_iStateID;
}

void CPlayer_Jump::State_Exit()
{
	/*m_pOwner->Open_Physics_Desc()->bGround = true;
	m_pOwner->Open_Physics_Desc()->bLanding = false;*/
}

void CPlayer_Jump::Jump()
{
	m_pOnwerRigidBody->Jump(m_pOwner->Open_Physics_Desc()->fJumpPower, m_pOwner->Open_Physics_Desc()->fJumpGravity);
}

void CPlayer_Jump::Land()
{
	m_pOnwerRigidBody->Land();

	m_pOwner->Open_Physics_Desc()->bDoubleJump = false;
	m_pOwner->Open_Physics_Desc()->bFall = false;
	m_pOwner->Open_Physics_Desc()->bJump = false;
	m_pOwner->Open_Physics_Desc()->bLanding = true;
	m_pOnwerTransform->Set_Ground(true);
}

_bool CPlayer_Jump::Is_Fall()
{
	/*if (m_pOnwerRigidBody->Is_Power_Zero(CRigidBody::TYPE::TYPE_VELOCITY)) {

		if(m_pOwner->Open_Physics_Desc()->bDoubleJump)
			m_pOwner->Animation_By_Type(CPlayer::STATE::FALL);

		m_pOwner->Open_Physics_Desc()->bFall = true;
		m_pOnwerRigidBody->Set_GravityPower(m_pOwner->Open_Physics_Desc()->fFallGravity);
	}*/

	return m_pOnwerRigidBody->Is_Power_Zero(CRigidBody::TYPE::TYPE_VELOCITY);
}

CPlayer_Jump* CPlayer_Jump::Create(CGameObject* pGameObject)
{
	CPlayer_Jump* pInstance = new CPlayer_Jump();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Jump");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Jump::Free()
{
	__super::Free();
}

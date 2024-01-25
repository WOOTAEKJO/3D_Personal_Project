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
	if(m_pOwner->Open_Physics_Desc()->bDoubleJump)
		m_pOwner->Animation_By_Type(CPlayer::STATE::DOUBLEJUMP);
	else
		m_pOwner->Animation_By_Type(CPlayer::STATE::JUMP);

	m_pOwner->Open_Physics_Desc()->bJump = true;
	Jump();

	Create_Particle(PARTICLE_JACKJUMP_TAG, GO_PARTICLENORMAL_TAG, m_pOwner,
		&m_pParticle, 1.f, &m_pOwner->Get_BodyModel()->Get_Bones());
	Particle_Loop_SetUp(m_pParticle, false);
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

	if (m_pOnwerRigidBody->Is_Power_Zero(CRigidBody::TYPE::TYPE_VELOCITY))
		return CPlayer::STATE::FALL;

	if (!m_pOwner->Open_Physics_Desc()->bDoubleJump)
	{
		if (m_pOnwerController->Key_Down(CPlayer::KEY_STATE::KEY_JUMP))
		{
			m_pOwner->Animation_By_Type(CPlayer::STATE::DOUBLEJUMP);
			m_pOwner->Open_Physics_Desc()->bDoubleJump = true;
			Jump();
			Create_Particle(PARTICLE_JACKJUMP_TAG, GO_PARTICLENORMAL_TAG, m_pOwner, &m_pParticle, 1.f,
				&m_pOwner->Get_BodyModel()->Get_Bones());
			Particle_Loop_SetUp(m_pParticle, false);
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

}

void CPlayer_Jump::Jump()
{
	m_pOnwerRigidBody->Set_GravityPower(m_pOwner->Open_Physics_Desc()->fJumpGravity);
	m_pOnwerRigidBody->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pOwner->Open_Physics_Desc()->fJumpPower,
		CRigidBody::TYPE::TYPE_VELOCITY);
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

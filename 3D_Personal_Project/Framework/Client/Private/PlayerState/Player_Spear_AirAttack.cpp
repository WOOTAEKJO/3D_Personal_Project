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
	Trans_Attack(true);
	m_pOwner->Animation_By_Type(CPlayer::STATE::AIR_ATTACK);

	Attack_Particle(1.f);

	m_pGameInstance->Play_Sound(L"Jack", L"Attack4Voice.ogg", CHANNELID::SOUND_PLAYER_VOICE, 3.f);
	
}

_uint CPlayer_Spear_AirAttack::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CPlayer_Spear_AirAttack::State_Tick(_float fTimeDelta)
{
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPlayer_Spear_AirAttack::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOnwerRigidBody->Is_Land())
	{
		m_pOwner->Create_Range_Bullet();
		if (m_bLand)
		{
			Create_Particle(PARTICLE_JACKLANDING_TAG, GO_PARTICLENORMAL_TAG, m_pOwner,
				&m_pParticle, 0.6f, & m_pOwner->Get_BodyModel()->Get_Bones());
			Particle_Loop_SetUp(m_pParticle, false);
			m_bLand = false;

			Create_Particle(PARTICLE_JACKELEC1_TAG, GO_PARTICLESPRITE_TAG, m_pOwner, &m_pElec[0], 3.f,
				&m_pOwner->Get_BodyModel()->Get_Bones());
			Particle_Loop_SetUp(m_pElec[0], false);
			Create_Particle(PARTICLE_JACKELEC2_TAG, GO_PARTICLESPRITE_TAG,
				m_pOwner, &m_pElec[1], 3.f, &m_pOwner->Get_BodyModel()->Get_Bones());
			Particle_Loop_SetUp(m_pElec[1], false);
			Create_Particle(PARTICLE_JACKELEC3_TAG, GO_PARTICLESPRITE_TAG,
				m_pOwner, &m_pElec[2], 3.f, &m_pOwner->Get_BodyModel()->Get_Bones());
			Particle_Loop_SetUp(m_pElec[2], false);

			m_pOwner->Camera_Shaking(0.3f, 0.3f, 0.13f);

			m_pGameInstance->Play_Sound(L"Jack", L"Attack4.ogg", CHANNELID::SOUND_PLAYER_ATTACK, 0.7f);
		}
		
		if (m_pOwnerModel->Is_Animation_Finished()) {

			m_pOwner->Open_Physics_Desc()->bDoubleJump = false;
			m_pOwner->Open_Physics_Desc()->bJump = false;

			return CPlayer::STATE::IDLE;
		}
	}

	if (m_pOnwerRigidBody->Is_Power_Zero(CRigidBody::TYPE::TYPE_VELOCITY)) {

		m_pOnwerRigidBody->Set_GravityPower(m_pOwner->Open_Physics_Desc()->fFallGravity * 3.f);
	}

	return m_iStateID;
}

void CPlayer_Spear_AirAttack::State_Exit()
{
	m_bLand = true;
	Trans_Attack(false);
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

	/*for (_uint i = 0; i < 3; i++)
	{
		Safe_Release(m_pElec[i]);
	}*/
}

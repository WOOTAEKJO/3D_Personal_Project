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
}

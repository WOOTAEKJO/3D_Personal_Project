#include "stdafx.h"
#include "..\Public\PlayerState\Player_Spear_Attack2.h"

#include "GameInstance.h"

CPlayer_Spear_Attack2::CPlayer_Spear_Attack2()
{
}

HRESULT CPlayer_Spear_Attack2::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Spear_Attack2::State_Enter()
{
	Trans_Attack(true);
	m_pOwner->Animation_By_Type(CPlayer::STATE::ATTACK2);
	Attack_Particle(1.f);
}

_uint CPlayer_Spear_Attack2::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Spear_Attack2::State_Tick(_float fTimeDelta)
{
	/*if (m_pOwnerModel->Is_Animation_Finished()) {

		Trans_Attack(false);

		m_fTime += fTimeDelta;

		if (m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
		{
			m_fTime = 0.f;
			return CPlayer::STATE::ATTACK3;
		}

		if (m_fTime > 0.3f)
		{
			m_fTime = 0.f;
			return CPlayer::STATE::IDLE;
		}
	}*/
	
	m_fTime += fTimeDelta;

	if (m_fTime > m_fAttackDelay)
	{
		if (!m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
			return CPlayer::STATE::IDLE;
		else {
			return m_pOwner->Get_NextAttackID();
		}
	}

	if (m_pOwnerModel->Is_Animation_Finished())
		return CPlayer::STATE::IDLE;

	m_pOwnerModel->Play_Animation(fTimeDelta, false);
	return m_iStateID;
}

_uint CPlayer_Spear_Attack2::State_Late_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

void CPlayer_Spear_Attack2::State_Exit()
{
	Trans_Attack(false);
	m_fTime = 0.f;
}


CPlayer_Spear_Attack2* CPlayer_Spear_Attack2::Create(CGameObject* pGameObject)
{
	CPlayer_Spear_Attack2* pInstance = new CPlayer_Spear_Attack2();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Spear_Attack2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Spear_Attack2::Free()
{
	__super::Free();
}

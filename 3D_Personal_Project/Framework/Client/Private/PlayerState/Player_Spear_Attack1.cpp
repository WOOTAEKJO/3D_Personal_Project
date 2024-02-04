#include "stdafx.h"
#include "..\Public\PlayerState\Player_Spear_Attack1.h"

#include "GameInstance.h"

#include "Utility_Effect.h"

CPlayer_Spear_Attack1::CPlayer_Spear_Attack1()
{
}

HRESULT CPlayer_Spear_Attack1::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Spear_Attack1::State_Enter()
{
	
	Trans_Attack(true);
	m_pOwner->Animation_By_Type(CPlayer::STATE::ATTACK1);
	Attack_Particle(1.f);

	m_pGameInstance->Play_Sound(L"Jack", L"Attack1Voice.ogg", CHANNELID::SOUND_PLAYER_VOICE, 3.f);
	

}

_uint CPlayer_Spear_Attack1::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Spear_Attack1::State_Tick(_float fTimeDelta)
{
	
	if (m_bSound)
	{
		m_fSoundTime += fTimeDelta;
		if (m_fSoundTime > 0.05f)
		{
			m_pGameInstance->Play_Sound(L"Jack", L"Attack1.ogg", CHANNELID::SOUND_PLAYER_ATTACK, 0.5f);
			m_bSound = false;
		}
	}

	m_fTime += fTimeDelta;

	if (m_fTime > m_fAttackDelay)
	{
		if(!m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
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

_uint CPlayer_Spear_Attack1::State_Late_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

void CPlayer_Spear_Attack1::State_Exit()
{
	Trans_Attack(false);
	m_fTime = 0.f;
	m_bSound = true;
	m_fSoundTime = 0.f;
}

CPlayer_Spear_Attack1* CPlayer_Spear_Attack1::Create(CGameObject* pGameObject)
{
	CPlayer_Spear_Attack1* pInstance = new CPlayer_Spear_Attack1();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Spear_Attack1");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Spear_Attack1::Free()
{
	__super::Free();
}

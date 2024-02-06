#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Dead.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

#include "Utility_Effect.h"

CHelico_Dead::CHelico_Dead()
{
}

HRESULT CHelico_Dead::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Dead::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::DEAD);
	
	CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_BOSS1BAT1_TAG, GO_PARTICLESPRITE_TAG,
		m_pOwner, nullptr, 1.f);
	CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_BOSS1BAT2_TAG, GO_PARTICLESPRITE_TAG,
		m_pOwner, nullptr, 1.f,false,false);
	m_pOwner->Set_Light_Activate(false);

	m_pGameInstance->Play_Sound(L"HelicoScarrow", L"Attack_StartVoice.ogg", CHANNELID::SOUND_BOSS_VOICE, 0.5f);
}

_uint CHelico_Dead::State_Priority_Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > 0.5f && m_bIntro)
	{
		m_pGameInstance->SetUp_Production(TEXT("Boss2Intro"));
		m_bIntro = false;
	}

	return m_iStateID;
}

_uint CHelico_Dead::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CHelico_Dead::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CHelico_Dead::State_Exit()
{
}

CHelico_Dead* CHelico_Dead::Create(CGameObject* pGameObject)
{
	CHelico_Dead* pInstance = new CHelico_Dead();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Dead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Dead::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Dead.h"
#include "StateMachine.h"

#include "Phantom.h"

#include "Utility_Effect.h"

CPhantom_Dead::CPhantom_Dead()
{
}

HRESULT CPhantom_Dead::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Dead::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::DEAD);
	
}

_uint CPhantom_Dead::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Dead::State_Tick(_float fTimeDelta)
{
	
	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(10, 110))
	{
		if (m_bParticle)
		{
			CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_BAT1_TAG, GO_PARTICLESPRITE_TAG,
				m_pOwner, nullptr, 9.9f,nullptr, true);
			CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_BAT2_TAG, GO_PARTICLESPRITE_TAG,
				m_pOwner, nullptr, 10.f, nullptr, true,false);

			m_pGameInstance->Play_Sound(L"BGM", L"Stage3Victory.ogg", CHANNELID::SOUND_BGM, 0.7f);
			m_pGameInstance->Play_Sound(L"Phantom", L"Dead.ogg", CHANNELID::SOUND_BOSS_VOICE, 1.f);
			
			m_bParticle = false;
		}
	}

	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(10, 180))
	{
		if (m_bSound)
		{
			m_pGameInstance->Play_Sound(L"Phantom", L"Dead2.ogg", CHANNELID::SOUND_BOSS_VOICE, 1.f);
			m_pOwner->Start_DeadTime();
			m_bSound = false;
		}
	}

	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(10, 320))
	{
		if (m_bSound2)
		{
			m_pGameInstance->Play_Sound(L"BGM", L"Stage2BGM.ogg", CHANNELID::SOUND_BGM, 0.7f, true);



			m_bSound2 = false;
		}

		dynamic_cast<CPhantom*>(m_pOwner)->Fall(fTimeDelta);
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Dead::State_Late_Tick(_float fTimeDelta)
{


	return m_iStateID;
}

void CPhantom_Dead::State_Exit()
{
	m_bSound = true;
	m_bSound2 = true;
	m_bParticle = true;
}

CPhantom_Dead* CPhantom_Dead::Create(CGameObject* pGameObject)
{
	CPhantom_Dead* pInstance = new CPhantom_Dead();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Dead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Dead::Free()
{
	__super::Free();

}

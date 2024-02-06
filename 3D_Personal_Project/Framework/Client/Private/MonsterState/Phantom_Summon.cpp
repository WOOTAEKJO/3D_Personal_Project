#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Summon.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Summon::CPhantom_Summon()
{
}

HRESULT CPhantom_Summon::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Summon::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::SUMMON);
	m_pGameInstance->Play_Sound(L"Phantom", L"SummonVoice.ogg", CHANNELID::SOUND_BOSS_VOICE, 1.f);
}

_uint CPhantom_Summon::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Summon::State_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CPhantom::STATE::SUMMON, 60.f))
	{
		if (m_bSound)
		{
			m_pGameInstance->Play_Sound(L"Phantom", L"SummonVoice2.ogg", CHANNELID::SOUND_BOSS_VOICE, 1.f);
			m_bSound = false;
		}
	}
	
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Summon::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
		return CPhantom::STATE::SUMMON_LOOP;

	return m_iStateID;
}

void CPhantom_Summon::State_Exit()
{
	m_bSound = true;
}

CPhantom_Summon* CPhantom_Summon::Create(CGameObject* pGameObject)
{
	CPhantom_Summon* pInstance = new CPhantom_Summon();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Summon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Summon::Free()
{
	__super::Free();

}

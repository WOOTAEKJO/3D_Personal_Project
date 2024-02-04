#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Intro.h"
#include "StateMachine.h"

#include "Phantom.h"
#include "Trigger.h"

CPhantom_Intro::CPhantom_Intro()
{
}

HRESULT CPhantom_Intro::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Intro::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::INTRO);

}

_uint CPhantom_Intro::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Intro::State_Tick(_float fTimeDelta)
{

	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CPhantom::STATE::INTRO, 120.f))
	{
		if (m_bAttack)
		{
			m_pGameInstance->Play_Sound(L"Phantom", L"IntroVoice.ogg", CHANNELID::SOUND_BOSS_VOICE, 1.5f, false);
			m_bAttack = false;
		}

	}
	
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Intro::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CPhantom::STATE::IDLE;
	}

	return m_iStateID;
}

void CPhantom_Intro::State_Exit()
{
	if (FAILED(m_pGameInstance->Add_Event(TEXT("Boss2Talk"), [this]() {

		m_pGameInstance->SetUp_Production(TEXT("Boss2Talk"));

		})))
		return;

	CTrigger::TRIGGER_DESC TriggerDesc = {};
	TriggerDesc.strEventName = TEXT("Boss2Talk");
	TriggerDesc.vPosition = _float4(22.9f, 2.f, 24.555f, 1.f);
	TriggerDesc.vScale = _float3(1.5f, 1.f, 1.5f);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_PLATEFORM]
		, GO_TRIGGER_TAG, &TriggerDesc)))
		return;

	m_bAttack = true;
}

CPhantom_Intro* CPhantom_Intro::Create(CGameObject* pGameObject)
{
	CPhantom_Intro* pInstance = new CPhantom_Intro();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Intro");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Intro::Free()
{
	__super::Free();

}

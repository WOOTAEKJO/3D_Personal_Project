#include "stdafx.h"
#include "..\Public\NPCState\NPC_Ready.h"

#include "Player.h"
#include "Crow.h"

#include "Monster.h"
#include "Trigger.h"

#include "GameMgr.h"

CNPC_Ready::CNPC_Ready()
{
}

HRESULT CNPC_Ready::Initialize(CGameObject* pGameObject)
{
	if (FAILED(__super::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNPC_Ready::State_Enter()
{
	m_pOwner->Set_TypeAnimIndex(CNPC::STATE::IDLE);
}

_uint CNPC_Ready::State_Priority_Tick(_float fTimeDelta)
{
	
	return m_iStateID;
}

_uint CNPC_Ready::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNPC_Ready::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNPC_Ready::State_Exit()
{
	if (FAILED(m_pGameInstance->Add_Event(TEXT("Battle2_Start"), [this]() {
		m_pGameInstance->Play_Sound(L"BGM", L"FightBGM.ogg", CHANNELID::SOUND_BGM, 0.7f, true);
		})))
		return;

		if (FAILED(m_pGameInstance->Add_Event(TEXT("Battle2_End"), [this]() {
			m_pGameInstance->Play_Sound(L"BGM", L"StageBGM.ogg", CHANNELID::SOUND_BGM, 0.7f, true);
			m_pGameInstance->Play_Sound(L"BGM", L"StageAmbiant.ogg", CHANNELID::SOUND_ENVIRONMENT, 0.7f, true);
			})))
			return;

	if (FAILED(m_pGameInstance->Add_Event(TEXT("Battle2"), [this]() {

		list<CGameObject*> listMonst = m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
			g_strLayerName[LAYER::LAYER_MONSTER]);

		CGameMgr::GAME_EVENT_DESC Desc = {};
		Desc.strStartEventName = TEXT("Battle2_Start");
		Desc.strEndEventName = TEXT("Battle2_End");

		CGameMgr::GetInstance()->Start_Game(Desc);

		_uint i = 0;
		for (auto& iter : listMonst)
		{
			if (i == 9)
				return;
			if (!dynamic_cast<CMonster*>(iter)->Get_DeadTime())
			{
				dynamic_cast<CMonster*>(iter)->Set_Activate();
				CGameMgr::GetInstance()->Add_GameToken(iter);
				++i;
			}
		}

		})))
		return;


	CTrigger::TRIGGER_DESC TriggerDesc = {};
	TriggerDesc.strEventName = TEXT("Battle2");
	TriggerDesc.vPosition = _float4(25.4f, 7.f, 42.2f, 1.f);
	TriggerDesc.vScale = _float3(3.f, 2.f, 3.f);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_PLATEFORM]
		, GO_TRIGGER_TAG, &TriggerDesc)))
		return ;
}

CNPC_Ready* CNPC_Ready::Create(CGameObject* pGameObject)
{
	CNPC_Ready* pInstance = new CNPC_Ready();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNPC_Ready");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNPC_Ready::Free()
{
	__super::Free();
}

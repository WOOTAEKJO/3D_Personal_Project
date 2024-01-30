#include "stdafx.h"
#include "..\Public\Boss1Talk.h"
#include "TargetCamera.h"

#include "HelicoScarrow.h"

CBoss1Talk::CBoss1Talk()
{

}

HRESULT CBoss1Talk::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CBoss1Talk::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC Boss1Info1 = {};
	Boss1Info1.fSensitivity = 0.2f;
	Boss1Info1.vOffset = Compute_Offset(TEXT("Boss1"), 0.4f, -0.3f, -0.5f);
	//Boss1Info1.iAnimIndex = 2;
	Boss1Info1.strActorTag = TEXT("Boss1");

	PRODUCTION_ACTOR_DESC Boss1Info2 = {};
	Boss1Info2.fSensitivity = 0.7f;
	Boss1Info2.vOffset = Boss1Info1.vOffset;
	Boss1Info2.strActorTag = TEXT("Boss1");

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(Boss1Info1);
	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(Boss1Info2);

	Camera_Target_Change(true);
}

void CBoss1Talk::Tick()
{
	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
	}
}

void CBoss1Talk::Render()
{
	
}

void CBoss1Talk::Exite()
{
	dynamic_cast<CHelicoScarrow*>(Find_Actor(TEXT("Boss1")))->Open_Status_Desc()->bTalk = false;

	Camera_Reset();
}

CBoss1Talk* CBoss1Talk::Create()
{
	CBoss1Talk* pInstance = new CBoss1Talk();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CBoss1Talk");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss1Talk::Free()
{
	__super::Free();
}

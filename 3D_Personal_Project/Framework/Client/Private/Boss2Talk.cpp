#include "stdafx.h"
#include "..\Public\Boss2Talk.h"
#include "TargetCamera.h"

#include "Phantom.h"

CBoss2Talk::CBoss2Talk()
{

}

HRESULT CBoss2Talk::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CBoss2Talk::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC Boss2Info1 = {};
	Boss2Info1.fSensitivity = 0.2f;
	Boss2Info1.vOffset = Compute_Offset(TEXT("Boss2"), 1.f, -2.5f, -2.f);
	Boss2Info1.strActorTag = TEXT("Boss2");
	Boss2Info1.fLookHeight = 12.f;

	PRODUCTION_ACTOR_DESC Boss2Info2 = {};
	Boss2Info2.fSensitivity = 0.7f;
	Boss2Info2.vOffset = Boss2Info1.vOffset;
	Boss2Info2.strActorTag = TEXT("Boss2");
	Boss2Info2.fLookHeight = Boss2Info1.fLookHeight;

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(Boss2Info1);
	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(Boss2Info2);

	Camera_Target_Change(true);
}

void CBoss2Talk::Tick()
{
	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
	}
}

void CBoss2Talk::Render()
{
	
}

void CBoss2Talk::Exite()
{
	Find_Actor(TEXT("Boss2"))->Get_Component<CStateMachine>()->Set_State(CPhantom::STATE::INTROEND);

	Camera_Reset();
}

CBoss2Talk* CBoss2Talk::Create()
{
	CBoss2Talk* pInstance = new CBoss2Talk();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CBoss2Talk");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss2Talk::Free()
{
	__super::Free();
}

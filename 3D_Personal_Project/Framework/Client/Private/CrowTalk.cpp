#include "stdafx.h"
#include "..\Public\CrowTalk.h"
#include "TargetCamera.h"

#include "Crow.h"

CCrowTalk::CCrowTalk()
{

}

HRESULT CCrowTalk::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CCrowTalk::Enter()
{
	SetUp_Camera();

	

	PRODUCTION_ACTOR_DESC CrowInfo1 = {};
	CrowInfo1.fSensitivity = 0.1f;
	CrowInfo1.vOffset = Compute_Offset(TEXT("Crow"), 0.1f, -0.2f, -0.5f);
	CrowInfo1.iAnimIndex = 2;
	CrowInfo1.strActorTag = TEXT("Crow");

	PRODUCTION_ACTOR_DESC CrowInfo2 = {};
	CrowInfo2.fSensitivity = 0.7f;
	CrowInfo2.vOffset = CrowInfo1.vOffset;
	CrowInfo2.strActorTag = TEXT("Crow");

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(CrowInfo2);
	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(CrowInfo2);

	Camera_Target_Change(true);
}

void CCrowTalk::Tick()
{
	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
	}
}

void CCrowTalk::Render()
{
	
}

void CCrowTalk::Exite()
{
	Find_Actor(TEXT("Crow"))->Get_Component<CStateMachine>()->Set_State(CCrow::STATE::FOLLOW);

	Camera_Reset();
}

CCrowTalk* CCrowTalk::Create()
{
	CCrowTalk* pInstance = new CCrowTalk();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CCrowTalk");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrowTalk::Free()
{
	__super::Free();
}

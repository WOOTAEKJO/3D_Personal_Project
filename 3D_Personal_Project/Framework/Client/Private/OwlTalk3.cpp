#include "stdafx.h"
#include "..\Public\OwlTalk3.h"
#include "TargetCamera.h"

#include "Owl.h"
#include "Player.h"

COwlTalk3::COwlTalk3()
{

}

HRESULT COwlTalk3::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void COwlTalk3::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC OwlInfo1 = {};
	OwlInfo1.fSensitivity = 0.6f;
	//OwlInfo1.vOffset = _float3(0.5f, -0.1f, 0.1f);
	OwlInfo1.vOffset = Compute_Offset(TEXT("Owl"), 0.1f, -0.1f, 0.5f);
	OwlInfo1.iAnimIndex = 2;
	OwlInfo1.strActorTag = TEXT("Owl");

	PRODUCTION_ACTOR_DESC OwlInfo2 = {};
	OwlInfo2.fSensitivity = 0.7f;
	OwlInfo2.vOffset = OwlInfo1.vOffset;
	OwlInfo2.strActorTag = TEXT("Owl");

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	//PlayerInfo.vOffset = _float3(-0.4f, -0.3f, 0.3f);
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(OwlInfo1);
	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(OwlInfo2);

	Camera_Target_Change(true);
}

void COwlTalk3::Tick()
{
	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
	}
}

void COwlTalk3::Render()
{
	
}

void COwlTalk3::Exite()
{
	//dynamic_cast<CCharacter*>()
	Find_Actor(TEXT("Owl"))->Get_Component<CStateMachine>()->Set_State(COwl::STATE::IDLE);

	Camera_Reset();
}

COwlTalk3* COwlTalk3::Create()
{
	COwlTalk3* pInstance = new COwlTalk3();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : COwlTalk3");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void COwlTalk3::Free()
{
	__super::Free();
}

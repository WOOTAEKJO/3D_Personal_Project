#include "stdafx.h"
#include "..\Public\Boss2Intro.h"
#include "TargetCamera.h"

#include "Phantom.h"

CBoss2Intro::CBoss2Intro()
{

}

HRESULT CBoss2Intro::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CBoss2Intro::Enter()
{
	SetUp_Camera();

	Find_Actor(TEXT("Boss2"))->Get_Component<CTransform>()->Set_State(CTransform::STATE::STATE_POS, _float4(24.223f, 2.f, 25.693f, 1.f));
	Find_Actor(TEXT("Boss2"))->Get_Component<CStateMachine>()->Set_State(CPhantom::STATE::INTRO);

	PRODUCTION_ACTOR_DESC CamInfo1 = {};
	CamInfo1.fSensitivity = 0.1f;
	CamInfo1.vOffset = Compute_Offset(TEXT("Cam"), 0.35f, 0.5f, -12.f);
	CamInfo1.strActorTag = TEXT("Cam");

	m_vecActorOrder.push_back(CamInfo1);

	Camera_Target_Change(true);
}

void CBoss2Intro::Tick()
{
	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
	}
}

void CBoss2Intro::Render()
{
	
}

void CBoss2Intro::Exite()
{
	Find_Actor(TEXT("Boss2"))->Get_Component<CStateMachine>()->Set_State(CPhantom::STATE::IDLE);

	Camera_Reset();
}

CBoss2Intro* CBoss2Intro::Create()
{
	CBoss2Intro* pInstance = new CBoss2Intro();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CBoss2Intro");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss2Intro::Free()
{
	__super::Free();
}

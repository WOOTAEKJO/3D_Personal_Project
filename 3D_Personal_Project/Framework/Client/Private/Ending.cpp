#include "stdafx.h"
#include "..\Public\Ending.h"
#include "TargetCamera.h"

#include "Phantom.h"

CEnding::CEnding()
{

}

HRESULT CEnding::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CEnding::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC CamInfo1 = {};
	CamInfo1.fSensitivity = 0.3f;
	CamInfo1.vOffset = Compute_Offset(TEXT("Cam"), -3.f, 3.f, -5.f);
	CamInfo1.strActorTag = TEXT("Cam");

	m_vecActorOrder.push_back(CamInfo1);

	Camera_Target_Change(true);
}

void CEnding::Tick()
{
	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
	}
}

void CEnding::Render()
{
	
}

void CEnding::Exite()
{

	//Camera_Reset();

	//m_pGameInstance->SetUp_Production(TEXT("Ending2"));
}

HRESULT CEnding::Ready_UI()
{
	return S_OK;
}

void CEnding::Ready_Font()
{
}

CEnding* CEnding::Create()
{
	CEnding* pInstance = new CEnding();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CEnding");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnding::Free()
{
	__super::Free();
}

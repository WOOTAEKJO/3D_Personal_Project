#include "stdafx.h"
#include "..\Public\Client_Production.h"

#include "TargetCamera.h"

CClient_Production::CClient_Production()
{

}

HRESULT CClient_Production::Initialize()
{
	/*m_pCamera = dynamic_cast<CTargetCamera*>(m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER::LAYER_CAMERA]).front());

	if (m_pCamera == nullptr)
		return E_FAIL;*/

	return S_OK;
}

void CClient_Production::Enter()
{
}

void CClient_Production::Tick()
{
}

void CClient_Production::Render()
{
}

void CClient_Production::Exite()
{
}

_bool CClient_Production::SetUp_Camera()
{
	m_pCamera = dynamic_cast<CTargetCamera*>(m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER::LAYER_CAMERA]).front());

	if (m_pCamera == nullptr)
		return false;

	return true;
}

void CClient_Production::Camera_Target_Change(_bool bSpring)
{
	if (m_vecActorOrder.empty())
		return;

	if (m_iCurrentOrderIndx >= m_vecActorOrder.size())
	{
		m_pGameInstance->Finish_Production();
		return;
	}
		
	PRODUCTION_ACTOR_DESC Infp = m_vecActorOrder[m_iCurrentOrderIndx];

	CGameObject* pActor = Find_Actor(Infp.strActorTag);

	m_pCamera->Change_Target(Infp.fSensitivity, true, pActor);
	m_pCamera->SetUp_Offset(Infp.vOffset);
	m_pCamera->CutSceneSpring(bSpring);

	if (Infp.iAnimIndex != -1)
		pActor->Get_Component<CModel>()->Set_AnimationIndex(Infp.iAnimIndex);

	++m_iCurrentOrderIndx;
}

void CClient_Production::Camera_Reset()
{
	m_pCamera->Change_Target(1.f, false, Find_Actor(TEXT("Player")));
	m_pCamera->Reset_Offset();
}

void CClient_Production::Free()
{
	__super::Free();

	Safe_Release(m_pCamera);
}

#include "stdafx.h"
#include "..\Public\Client_Production.h"

#include "TargetCamera.h"
#include "UI.h"
#include "UI_Dissolve.h"
#include "UI_Dissolve.h"

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
	m_pCamera->SetUp_LookAt_Height(Infp.fLookHeight);

	if (Infp.iAnimIndex != -1)
		pActor->Get_Component<CModel>()->Set_AnimationIndex(Infp.iAnimIndex);

	++m_iCurrentOrderIndx;
}

void CClient_Production::Camera_Reset()
{
	m_pCamera->Change_Target(1.f, false, Find_Actor(TEXT("Player")));
	m_pCamera->Reset_Offset();
	m_pCamera->Reset_LookAt_Height();
}

_float3 CClient_Production::Compute_Offset(const wstring& strActorTag, _float fX, _float fY, _float fZ)
{
	CGameObject* pActor = Find_Actor(strActorTag);
	if(pActor == nullptr)
		return _float3();

	CTransform* pActorTransform = pActor->Get_Component<CTransform>();

	//_vector vPos = pActorTransform->Get_State(CTransform::STATE::STATE_POS);
	_vector vRight = XMVector3Normalize(pActorTransform->Get_State(CTransform::STATE::STATE_RIGHT)) * fX;
	_vector vUp = XMVector3Normalize(pActorTransform->Get_State(CTransform::STATE::STATE_UP)) * fY;
	_vector vLook = XMVector3Normalize( pActorTransform->Get_State(CTransform::STATE::STATE_LOOK)) * fZ;
	

	_float3 vOffset;

	XMStoreFloat3(&vOffset, vLook + vRight + vUp);

	return vOffset;
}

_float3 CClient_Production::Compute_Offset_Transform(CTransform* pActorTramsform, _float fLook, _float fRight)
{
	if(pActorTramsform == nullptr)
		return _float3();

	_vector vLook = XMVector3Normalize(pActorTramsform->Get_State(CTransform::STATE::STATE_LOOK)) * fLook;
	_vector vRight = XMVector3Normalize(pActorTramsform->Get_State(CTransform::STATE::STATE_RIGHT)) * fRight;

	_float3 vOffset;

	XMStoreFloat3(&vOffset, vLook + vRight);

	return vOffset;
}

HRESULT CClient_Production::Ready_UI()
{

	return S_OK;
}

void CClient_Production::Ready_Font()
{

}

CUI* CClient_Production::Add_UI(_float2 vCenterPos, _float2 vScale, const wstring& strTextureTag, const wstring& strProtoTag)
{
	CGameObject* pUi = nullptr;

	CUI::UI_DESC Desc = {};

	Desc.strTextureTag = strTextureTag;
	Desc.vCenterPos = vCenterPos;
	Desc.vScale = vScale;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_UI]
		, strProtoTag, &Desc, reinterpret_cast<CGameObject**>(&pUi)))) return nullptr;

	return dynamic_cast<CUI*>(pUi);
}

CUI* CClient_Production::Add_UI_Dissolve(_float2 vCenterPos, _float2 vScale, const wstring& strTextureTag,
	const wstring& strDissolveTexture, const wstring& strProtoTag)
{
	CGameObject* pUi = nullptr;

	CUI_Dissolve::UI_DISSOLVE_DESC Desc = {};

	Desc.strTextureTag = strTextureTag;
	Desc.strDissolveTexture = strDissolveTexture;
	Desc.vCenterPos = vCenterPos;
	Desc.vScale = vScale;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_UI]
		, strProtoTag, &Desc, reinterpret_cast<CGameObject**>(&pUi)))) return nullptr;

	return dynamic_cast<CUI*>(pUi);
}

HRESULT CClient_Production::Add_GROUP(const wstring& strGroupTag, CUI* pUI)
{
	if (pUI == nullptr)
		return E_FAIL;

	list<CUI*>* pList = Find_UIGROUP(strGroupTag);
	
	if (pList == nullptr)
	{
		list<CUI*> listUI;
		listUI.push_back(pUI);

		m_mapUI.emplace(strGroupTag, listUI);
	}
	else {
		pList->push_back(pUI);
	}

	return S_OK;
}

list<CUI*>* CClient_Production::Find_UIGROUP(const wstring& strGroupTag)
{
	auto& iter = m_mapUI.find(strGroupTag);
	if (iter == m_mapUI.end())
		return nullptr;

	return &iter->second;
}

void CClient_Production::SetUp_UI_Group(const wstring& strGroupTag)
{
	if (CUtility_String::Compare_WString(m_strActivateGroupTag, strGroupTag))
	{
		return;
	}
	else {
		list<CUI*>* pList = Find_UIGROUP(strGroupTag);
		if (pList == nullptr)
			return;

		for (auto& iter : *pList)
		{
			iter->Set_Render(true);
		}

		m_strActivateGroupTag = strGroupTag;
	}
}

void CClient_Production::Clear_UI_Group()
{
	list<CUI*>* pList = Find_UIGROUP(m_strActivateGroupTag);
	if (pList == nullptr)
		return;

	for (auto& iter : *pList)
	{
		iter->Set_Render(false);
	}

	m_bFontRender = false;
}

void CClient_Production::Is_Font_Render()
{
	if (m_iCurrentUIOrderIndx >= m_vecUIOrder.size())
		return;

	if (dynamic_cast<CUI_Dissolve*>(Find_UIGROUP(m_vecUIOrder[m_iCurrentUIOrderIndx])->front())->Get_Ready_Ok())
	{
		m_bFontRender = true;
		++m_iCurrentUIOrderIndx;
	}
}

void CClient_Production::RenderUI()
{
	_uint iSize = m_vecUIOrder.size();

	if (m_iCurrentUIOrderIndx >= iSize)
	{
		Clear_UI_Group();
		return;
	}

	if (m_iCurrentUIOrderIndx != 0)
	{
		++m_iCurrentFontIndx;
		Clear_UI_Group();
	}

	SetUp_UI_Group(m_vecUIOrder[m_iCurrentUIOrderIndx]);	
}

void CClient_Production::RenderFont()
{
	if (!m_bFontRender)
		return;

	if (m_iCurrentFontIndx >= m_vecFont.size())
		return;

	m_pGameInstance->Render_Font(m_vecFont[m_iCurrentFontIndx].iFontTag,
		m_vecFont[m_iCurrentFontIndx].strText,
		m_vecFont[m_iCurrentFontIndx].vPosition,
		XMLoadFloat4(&m_vecFont[m_iCurrentFontIndx].vColor),
		m_vecFont[m_iCurrentFontIndx].fScale,
		m_vecFont[m_iCurrentFontIndx].vOrigin,
		m_vecFont[m_iCurrentFontIndx].fRotation);
}

void CClient_Production::Free()
{
	__super::Free();

	Safe_Release(m_pCamera);
}

#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"

#include "Level_Loading.h"

#include "BackGround.h"
#include "UI.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	if (GetKeyState('1') & 0x8000)
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING,CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}

	if (GetKeyState('2') & 0x8000)
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_TOOL))))
			return;
	}
}

HRESULT CLevel_Logo::Render()
{
	//SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	/*CBackGround::BACKGROUND_DESC BackGroundDesc = {};

	BackGroundDesc.fX = Client::g_iWinSizeX * 0.5f;
	BackGroundDesc.fY = Client::g_iWinSizeY * 0.5f;
	BackGroundDesc.fSizeX = g_iWinSizeX;
	BackGroundDesc.fSizeY = g_iWinSizeY;
	BackGroundDesc.fSpeedPerSec = 10.f;
	BackGroundDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_LOGO, strLayerTag, TEXT("Prototype_GameObject_BackGround"),&BackGroundDesc)))
		return E_FAIL;*/

	CUI::UI_DESC Desc = {};

	Desc.strTextureTag = UI_MAIN_TAG;
	Desc.vCenterPos = _float2(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	Desc.vScale = _float2((_float)g_iWinSizeX, (_float)g_iWinSizeY);
	Desc.bRender = true;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_UI]
		, GO_UICHATBOX_TAG, &Desc))) return E_FAIL;
	
	Desc.strTextureTag = UI_LOGO_TAG;
	Desc.vCenterPos = _float2((g_iWinSizeX * 0.5f), (g_iWinSizeY * 0.3f));
	Desc.vScale = _float2(800.f, 350.f);
	Desc.bRender = true;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_UI]
		, GO_UICHATBOX_TAG, &Desc))) return E_FAIL;
	
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}
	return pInstance; 
}

void CLevel_Logo::Free()
{
	__super::Free();

}

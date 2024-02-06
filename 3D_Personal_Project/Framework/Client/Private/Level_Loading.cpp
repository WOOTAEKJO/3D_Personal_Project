#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"

#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_Tool.h"
#include "Level_Boss1.h"
#include "Level_Boss2.h"

#include "UI.h"
#include "UI_Move.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	/* 추후 로딩 레벨이 끝나면 원래 목적으로 뒀던 레벨로 넘어가기위해서. */
	m_eNextLevelID = eNextLevelID;

	/* 메인스레드로 대충 로드한다. */
	/* 로딩용 자원을 로드한다. */
	/* 로딩레벨에서 보여줘야할 객체들을 생성한다.(배경, 일러스트, 로딩바) */

	/* 추가적인 스레드를 생성하여 eNextLevelID에 필요한 자원들을 로드한다. */

	m_pGameInstance->Set_Current_Level(m_eNextLevelID);

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	m_pGameInstance->Collision_Clear();

	if (FAILED(Ready_Layer_BackGround(TEXT("BackGround"))))
		return E_FAIL;

	m_pGameInstance->Play_Sound(L"BGM", L"LoadingBGM.ogg", CHANNELID::SOUND_BGM, 1.f);
	//m_pGameInstance->Play_Sound(L"BGM", L"LogoBGM.ogg", CHANNELID::SOUND_BGM, 1.f, true);

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	if (true == m_pLoader->isFinished())
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			CLevel*		pNewLevel = { nullptr };

			

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_TOOL:
				pNewLevel = CLevel_Tool::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_BOSS1:
				pNewLevel = CLevel_Boss1::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_BOSS2:
				pNewLevel = CLevel_Boss2::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pNewLevel)
				return;

			if (FAILED(m_pGameInstance->Open_Level(m_eNextLevelID, pNewLevel)))
				return;
		}
	}
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Print_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	CUI::UI_DESC Desc = {};

	wstring strLoadingTag;

	if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL_BOSS2)
		strLoadingTag = UI_LOADING2_TAG;
	else
		strLoadingTag = UI_LOADING1_TAG;

	Desc.strTextureTag = strLoadingTag;
	Desc.vCenterPos = _float2(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	Desc.vScale = _float2((_float)g_iWinSizeX, (_float)g_iWinSizeY);
	Desc.bRender = true;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_LOADING, g_strLayerName[LAYER::LAYER_UI]
		, GO_UICHATBOX_TAG, &Desc))) return E_FAIL;

	Desc.strTextureTag = UI_LOADINGHOLDER_TAG;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_LOADING, g_strLayerName[LAYER::LAYER_UI]
		, GO_UICHATBOX_TAG, &Desc))) return E_FAIL;

	CUI_Move::UI_MOVE_DESC Move_Desc = {};

	Move_Desc.strTextureTag = UI_LOADINGLOGO_TAG;
	Move_Desc.vCenterPos = _float2(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	Move_Desc.vScale = _float2((_float)g_iWinSizeX, (_float)g_iWinSizeY);
	Move_Desc.bRender = true;
	Move_Desc.fSpeed = 7.f;
	Move_Desc.fAngle = 0.f;
	Move_Desc.vDir = _float3(-1.f, 0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_LOADING, g_strLayerName[LAYER::LAYER_UI]
		, GO_UIMOVE_TAG, &Move_Desc))) return E_FAIL;

	Move_Desc.strTextureTag = UI_HEADDEATH_TAG;
	Move_Desc.vCenterPos = _float2(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	Move_Desc.vScale = _float2((_float)g_iWinSizeX, (_float)g_iWinSizeY);
	Move_Desc.bRender = true;
	Move_Desc.fSpeed = 7.f;
	Move_Desc.fAngle = 0.f;
	Move_Desc.vDir = _float3(1.f, 0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_LOADING, g_strLayerName[LAYER::LAYER_UI]
		, GO_UIMOVE_TAG, &Move_Desc))) return E_FAIL;

	Move_Desc.strTextureTag = UI_SPINNER_TAG;
	Move_Desc.vCenterPos = _float2((_float)g_iWinSizeX - (g_iWinSizeX * 0.1f), (_float)g_iWinSizeY - g_iWinSizeY * 0.1f);
	Move_Desc.vScale = _float2(100.f, 100.f);
	Move_Desc.bRender = true;
	Move_Desc.fSpeed = 0.f;
	Move_Desc.fAngle = XMConvertToRadians(-90.f);
	Move_Desc.vDir = _float3(1.f, 0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_LOADING, g_strLayerName[LAYER::LAYER_UI]
		, GO_UIMOVE_TAG, &Move_Desc))) return E_FAIL;

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevelID)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}
	return pInstance; 
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}

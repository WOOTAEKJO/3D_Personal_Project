#include "stdafx.h"
#include "..\Public\Level_Boss2.h"

#include "DynamicCamera.h"
#include "TargetCamera.h"

#include "GameInstance.h"
#include "DataMgr.h"

#include "ObjectMesh_Demo.h"
#include "AnimMesh_Demo.h"

#include "Monster.h"
#include "Utility_Effect.h"

CLevel_Boss2::CLevel_Boss2(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Boss2::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(g_strLayerName[LAYER_BACKGROUND])))
		return E_FAIL;

	if (FAILED(Ready_Layer_Plateform(g_strLayerName[LAYER_PLATEFORM])))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(g_strLayerName[LAYER_PLAYER])))
		return E_FAIL;

	if (FAILED(CDataMgr::GetInstance()->Level_Object_Load("../Bin/Data/Object/Stage3.bin")))
		return E_FAIL;
	/*if (FAILED(CDataMgr::GetInstance()->Level_Object_Instancing_Load("../Bin/Data/Object/Instancing/Stage3.bin")))
		return E_FAIL;*/
		
	if (FAILED(Ready_Layer_Camera(g_strLayerName[LAYER_CAMERA])))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(g_strLayerName[LAYER_MONSTER])))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER_BULLET, COLLIDER_LAYER::COL_MONSTER))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_MONSTER_BULLET))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_MONSTER))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER_BULLET, COLLIDER_LAYER::COL_TRIGGER_BULLET))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_MONSTER, COLLIDER_LAYER::COL_TRIGGER_BULLET))) return E_FAIL;

	CUtility_Effect::Create_Particle_Stage(m_pGameInstance, PARTICLE_STAGE3IDLE_TAG, _float4(16.739f, 3.5f, 18.639f, 1.f),
		nullptr, nullptr);

	CUtility_Effect::Create_Effect_Normal(m_pGameInstance, TEX_WATER_TAG, GO_EFFECTWATER_TAG, nullptr,
		XMVectorSet(16.739, 7.f, 18.639, 1.f), nullptr, 0.f, _float2(25.f, 25.f));
	
	m_pGameInstance->Fog_SetUp(_float2(0.f, 30.f), _float4(0.4f, 1.f, 1.f, 0.5f));

	SHADOW_LIGHT_DESC Shadow_Desc = {};
	Shadow_Desc.vPos = _float4(30.f, 30.f, 30.f, 1.f);
	Shadow_Desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	Shadow_Desc.vUpDir = _float4(0.f, 1.f, 0.f, 0.f);

	Shadow_Desc.fFov = XMConvertToRadians(60.f);
	Shadow_Desc.fAspect = ((_float)g_iWinSizeX / g_iWinSizeY);
	Shadow_Desc.fNear = 0.1f;
	Shadow_Desc.fFar = 700.f;

	m_pGameInstance->Get_ShadowLight()->Set_Light_Desc(Shadow_Desc);
	// 그림자 빛 세팅

	m_pGameInstance->Play_Sound(L"BGM", L"Stage3BGM.ogg", CHANNELID::SOUND_BGM, 0.7f, true);

	return S_OK; 
}

void CLevel_Boss2::Tick(_float fTimeDelta)
{
	
}

HRESULT CLevel_Boss2::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	
	return S_OK;
}

HRESULT CLevel_Boss2::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, G0_TERRAIN_TAG)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_SKYBOX_TAG)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_Boss2::Ready_Layer_Player(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, ANIMMODEL_JACK_TAG)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_NPC], ANIMMODEL_CROW_TAG)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss2::Ready_Layer_Plateform(const wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Boss2::Ready_Layer_Camera(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_TARGETCAMERA_TAG)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, G0_DCAMERA_TAG)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Boss2::Ready_Layer_Monster(const wstring& strLayerTag)
{

	return S_OK;
}

CLevel_Boss2 * CLevel_Boss2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Boss2*		pInstance = new CLevel_Boss2(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Boss2");
		Safe_Release(pInstance);
	}
	return pInstance; 
}

void CLevel_Boss2::Free()
{
	__super::Free();
}

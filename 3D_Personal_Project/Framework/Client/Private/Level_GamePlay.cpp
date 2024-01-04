#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "DynamicCamera.h"
#include "TargetCamera.h"

#include "GameInstance.h"
#include "DataMgr.h"

#include "ObjectMesh_Demo.h"
#include "AnimMesh_Demo.h"

#include "Monster.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(g_strLayerName[LAYER_BACKGROUND])))
		return E_FAIL;

	if (FAILED(Ready_Layer_Plateform(g_strLayerName[LAYER_PLATEFORM])))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(g_strLayerName[LAYER_PLAYER])))
		return E_FAIL;

	if (FAILED(CDataMgr::GetInstance()->Level_Object_Load("../Bin/Data/Object/Stage1.bin")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(g_strLayerName[LAYER_CAMERA])))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(g_strLayerName[LAYER_MONSTER])))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER_BULLET, COLLIDER_LAYER::COL_MONSTER))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_MONSTER_BULLET))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDET_LAYER::COL_PLAYER, COLLIDET_LAYER::COL_MONSTER))) return E_FAIL;

	return S_OK; 
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, G0_TERRAIN_TAG)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, GO_SKYBOX_TAG)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, ANIMMODEL_JACK_TAG)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, ANIMMODEL_CROW_TAG)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Plateform(const wstring& strLayerTag)
{

	/*json jLoad;

	if (FAILED(CJson_Utility::Load_Json(JSON_STAGE1_PATH, jLoad)))
		return E_FAIL;

	for (auto& iter : jLoad["Demo"])
	{
		CGameObject* pObject_Demo = nullptr;

		if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_PLATEFORM_TAG,
			nullptr, reinterpret_cast<CGameObject**>(&pObject_Demo))))
			return E_FAIL;

		pObject_Demo->Load_FromJson(iter);

	}*/

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring& strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, G0_DCAMERA_TAG)))
		return E_FAIL;*/

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, GO_TARGETCAMERA_TAG)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const wstring& strLayerTag)
{
	
	/*CMonster::MONSTER_DESC Monster_Desc = {};

	Monster_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Monster_Desc.fSpeedPerSec = 5.f;
	Monster_Desc.vPos = _float4(55.f, 0.f, 65.f, 1.f);
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, GO_SPOOKETON_TAG, &Monster_Desc, nullptr)))
		return E_FAIL;

	Monster_Desc.vPos = _float4(60.f, 0.f, 60.f, 1.f);
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, GO_SPOOKETON_TAG, &Monster_Desc, nullptr)))
		return E_FAIL;

	Monster_Desc.vPos = _float4(50.f, 0.f, 70.f, 1.f);
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, GO_SPOOKETON_TAG, &Monster_Desc, nullptr)))
		return E_FAIL;*/

	/*for (_uint i = 0; i < 3; i++)
	{
		CMonster::MONSTER_DESC Monster_Desc = {};

		Monster_Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Monster_Desc.fSpeedPerSec = 5.f;
		Monster_Desc.vPos = _float4(50.f + (5.f * i), 0.f, 20.f + (5.f * i), 1.f);
		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, ANIMMODEL_SPOOKETON_TAG, &Monster_Desc, nullptr)))
			return E_FAIL;
	}*/


	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}
	return pInstance; 
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}

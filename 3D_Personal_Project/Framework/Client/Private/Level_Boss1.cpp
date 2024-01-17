#include "stdafx.h"
#include "..\Public\Level_Boss1.h"

#include "DynamicCamera.h"
#include "TargetCamera.h"

#include "GameInstance.h"
#include "DataMgr.h"

#include "ObjectMesh_Demo.h"
#include "AnimMesh_Demo.h"

#include "Monster.h"

CLevel_Boss1::CLevel_Boss1(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Boss1::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(g_strLayerName[LAYER_BACKGROUND])))
		return E_FAIL;

	if (FAILED(Ready_Layer_Plateform(g_strLayerName[LAYER_PLATEFORM])))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(g_strLayerName[LAYER_PLAYER])))
		return E_FAIL;

	if (FAILED(CDataMgr::GetInstance()->Level_Object_Load("../Bin/Data/Object/Stage2.bin")))
		return E_FAIL;
	if (FAILED(CDataMgr::GetInstance()->Level_Object_Instancing_Load("../Bin/Data/Object/Instancing/Stage2.bin")))
		return E_FAIL;
		
	if (FAILED(Ready_Layer_Camera(g_strLayerName[LAYER_CAMERA])))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(g_strLayerName[LAYER_MONSTER])))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER_BULLET, COLLIDER_LAYER::COL_MONSTER))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_MONSTER_BULLET))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_MONSTER))) return E_FAIL;
	
	return S_OK; 
}

void CLevel_Boss1::Tick(_float fTimeDelta)
{
}

HRESULT CLevel_Boss1::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	
	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, G0_TERRAIN_TAG)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_SKYBOX_TAG)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_Player(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, ANIMMODEL_JACK_TAG)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, ANIMMODEL_CROW_TAG)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_Plateform(const wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_Camera(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_TARGETCAMERA_TAG)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, G0_DCAMERA_TAG)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Boss1::Ready_Layer_Monster(const wstring& strLayerTag)
{

	return S_OK;
}

CLevel_Boss1 * CLevel_Boss1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Boss1*		pInstance = new CLevel_Boss1(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Boss1");
		Safe_Release(pInstance);
	}
	return pInstance; 
}

void CLevel_Boss1::Free()
{
	__super::Free();
}

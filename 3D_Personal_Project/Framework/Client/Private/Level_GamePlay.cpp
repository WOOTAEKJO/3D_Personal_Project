#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "Level_Loading.h"

#include "DynamicCamera.h"
#include "TargetCamera.h"

#include "GameInstance.h"
#include "DataMgr.h"

#include "ObjectMesh_Demo.h"
#include "AnimMesh_Demo.h"

#include "Monster.h"

#include "Trigger.h"

#include "Plateform_Trap.h"

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
	if (FAILED(CDataMgr::GetInstance()->Level_Object_Instancing_Load("../Bin/Data/Object/Instancing/Stage1.bin")))
		return E_FAIL;
		
	if (FAILED(Ready_Layer_Camera(g_strLayerName[LAYER_CAMERA])))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(g_strLayerName[LAYER_MONSTER])))
		return E_FAIL;

	if (FAILED(Ready_Trigger()))
		return E_FAIL;

	if (FAILED(Ready_LightDesc()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER_BULLET, COLLIDER_LAYER::COL_MONSTER))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_MONSTER_BULLET))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_MONSTER))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_MONSTER, COLLIDER_LAYER::COL_MONSTER))) return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_TRIGGER))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Pair_Collision(COLLIDER_LAYER::COL_PLAYER, COLLIDER_LAYER::COL_TRAP))) return E_FAIL;
	

	
	return S_OK; 
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	if (m_bNextLevel)
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_BOSS1))))
			return;
	}

	/*if (m_pGameInstance->Key_Down(DIK_1))
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_BOSS1))))
			return;
	}*/

	if (m_pGameInstance->Key_Down(DIK_1))
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_BOSS2))))
			return;
	}
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, G0_TERRAIN_TAG)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_SKYBOX_TAG)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, ANIMMODEL_JACK_TAG)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, ANIMMODEL_CROW_TAG)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Plateform(const wstring& strLayerTag)
{
	// 일단 급하니 툴 거치지 않고 하드코딩함
	CPlateform_Trap::PLATEFORM_TRAP_DESC TrapDesc = {};
	TrapDesc.strModelTag = MODEL_WOODFUCKER_TAG;
	TrapDesc.vPos = _float4(14.004f, 10.f, 23.227f,1.f);
	TrapDesc.fAngle = _float3(0.f, 0.f, XMConvertToRadians(30.f));
	TrapDesc.fAmplitude = 1.01f;
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_PLATEFORM_TRAP_TAG,
		&TrapDesc)))
		return E_FAIL;

	TrapDesc.vPos = _float4(13.990f, 10.f, 25.996f, 1.f);
	TrapDesc.fAngle = _float3(0.f, 0.f, XMConvertToRadians(-30.f));
	TrapDesc.fAmplitude = 1.01f;
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_PLATEFORM_TRAP_TAG,
		&TrapDesc)))
		return E_FAIL;

	TrapDesc.vPos = _float4(15.407f, 10.f, 28.781f, 1.f);
	TrapDesc.fAngle = _float3(0.f, XMConvertToRadians(30.f), XMConvertToRadians(35.f));
	TrapDesc.fAmplitude = 1.02f;
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_PLATEFORM_TRAP_TAG,
		&TrapDesc)))
		return E_FAIL;

	TrapDesc.vPos = _float4(17.109f, 10.f, 30.635f, 1.f);
	TrapDesc.fAngle = _float3(0.f, XMConvertToRadians(35.f), XMConvertToRadians(-20.f));
	TrapDesc.fAmplitude = 1.005f;
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_PLATEFORM_TRAP_TAG,
		&TrapDesc)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring& strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, G0_DCAMERA_TAG)))
		return E_FAIL;*/

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), strLayerTag, GO_TARGETCAMERA_TAG)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const wstring& strLayerTag)
{
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Trigger()
{
	if (FAILED(m_pGameInstance->Add_Event(TEXT("Portal_Boss1"), [this]() {
		this->Set_NextLevel();
		})))
		return E_FAIL;

	CTrigger::TRIGGER_DESC TriggerDesc = {};
	TriggerDesc.strEventName = TEXT("Portal_Boss1");
	TriggerDesc.vPosition = _float4(46.f, 9.f, 34.f, 1.f);
	TriggerDesc.vScale = _float3(1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_PLATEFORM]
		, GO_TRIGGER_TAG,&TriggerDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Event(TEXT("Battle1"), [this]() {
		list<CGameObject*> listMonst = m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
			g_strLayerName[LAYER::LAYER_MONSTER]);

		for (auto& iter : listMonst)
		{
			dynamic_cast<CMonster*>(iter)->Set_Activate();
		}
		
		})))
		return E_FAIL;

	TriggerDesc.strEventName = TEXT("Battle1");
	TriggerDesc.vPosition = _float4(13.5f, 7.f, 13.2f, 1.f);
	TriggerDesc.vScale = _float3(5.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_PLATEFORM]
		, GO_TRIGGER_TAG, &TriggerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_LightDesc()
{
	LIGHT_DESC LightDesc = {};

	LightDesc.eType = LIGHT_DESC::LIGHT_TYPE::TYPE_DIRECTION;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

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

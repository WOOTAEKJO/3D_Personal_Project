#include "stdafx.h"
#include "..\Public\Level_Tool.h"

#include "../Public/ImGuiMgr.h"

#include "DynamicCamera.h"
#include "GameInstance.h"

CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Tool::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Camera"))))
		return E_FAIL;

	if (FAILED(Ready_LightDesc()))
		return E_FAIL;

	if (FAILED(CImGuiMgr::GetInstance()->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	m_pGameInstance->Fog_SetUp(_float2(0.f, 1000.f), _float4(1.f, 1.f, 1.f, 1.f));

	return S_OK;
}

void CLevel_Tool::Tick(_float fTimeDelta)
{
}

HRESULT CLevel_Tool::Render()
{
	SetWindowText(g_hWnd, TEXT("Tool"));

	if (FAILED(CImGuiMgr::GetInstance()->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, strLayerTag, TEXT("Prototype_GameObject_Terrain_Demo"))))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Camera(const wstring& strLayerTag)
{

	CDynamicCamera::DYNAMICCAMERADESC DynamicCameraDesc;

	DynamicCameraDesc.fMouseSensitivity = 0.1f;
	DynamicCameraDesc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
	DynamicCameraDesc.vAte = _float4(0.f, 0.f, 0.f, 1.f);
	DynamicCameraDesc.fFovy = XMConvertToRadians(60.f);
	DynamicCameraDesc.fAspect = ((_float)g_iWinSizeX) / g_iWinSizeY;
	DynamicCameraDesc.fNear = 0.1f;
	DynamicCameraDesc.fFar = 1000.f;
	DynamicCameraDesc.fSpeedPerSec = 5.f;
	DynamicCameraDesc.fRotationPerSec = XMConvertToRadians(180.f);

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, TEXT("Prototype_GameObject_DynamicCamera"), &DynamicCameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_LightDesc()
{
	LIGHT_DESC LightDesc = {};

	LightDesc.eType = LIGHT_DESC::LIGHT_TYPE::TYPE_DIRECTION;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	/*for (_uint i = 0; i < 10; i++)
	{
		LIGHT_DESC LightDesc = {};
	
		LightDesc.eType = LIGHT_DESC::TYPE_POINT;
		LightDesc.vPos = _float4(10.f + (i * 5.f),0.f,10.f + (i * 5.f),1.f);
		LightDesc.fRange = 0.6f;
		LightDesc.vDiffuse = _float4(0.8f, 0.8f, 0.f, 1.f);
		LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.vSpecular = LightDesc.vDiffuse;

		if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
			return E_FAIL;
	}*/

	return S_OK;
}

CLevel_Tool* CLevel_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Tool* pInstance = new CLevel_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Tool::Free()
{
	__super::Free();

	CImGuiMgr::GetInstance()->DestroyInstance();

}

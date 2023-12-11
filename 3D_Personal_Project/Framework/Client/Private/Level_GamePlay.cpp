#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "DynamicCamera.h"

#include "GameInstance.h"

#include "ObjectMesh_Demo.h"
#include "AnimMesh_Demo.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(TEXT("Object"))))
		return E_FAIL;

	return S_OK; 
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("�����÷��̷����Դϴ�."));
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, G0_TERRAIN_TAG)))
		return E_FAIL;*/
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring& strLayerTag)
{
	
	CDynamicCamera::DYNAMICCAMERADESC DynamicCameraDesc;

	DynamicCameraDesc.fMouseSensitivity = 0.1f;
	DynamicCameraDesc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
	DynamicCameraDesc.vAte = _float4(0.f, 0.f, 0.f, 1.f);
	DynamicCameraDesc.fFovy = XMConvertToRadians(60.f);
	DynamicCameraDesc.fAspect = ((_float)g_iWinSizeX) / g_iWinSizeY;
	DynamicCameraDesc.fNear = 0.1f;
	DynamicCameraDesc.fFar = 1000.f;
	DynamicCameraDesc.fSpeedPerSec = 30.f;
	DynamicCameraDesc.fRotationPerSec = XMConvertToRadians(180.f);

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, G0_DCAMERA_TAG, &DynamicCameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Object(const wstring& strLayerTag)
{
	/*CObjectMesh_Demo::OBDEMOVALUE ObjectDemoValue;

	ObjectDemoValue.strModelTag = strModelTag;
	ObjectDemoValue.vPos = vPickPos;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, G0_OBJECTMESH_DEMO_TAG,
		&ObjectDemoValue)))
		return E_FAIL;*/

	CAnimMesh_Demo::ANIMDEMOVALUE AnimMeshDemoValue;

	AnimMeshDemoValue.strModelTag = ANIMMODEL_FIONA_TAG;
	AnimMeshDemoValue.vPos = _float4(0.f, 0.f, 0.f, 1.f);

	for (_uint i = 0; i < 20; i++) {
		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, G0_ANIMMESH_DEMO_TAG,
			&AnimMeshDemoValue)))
			return E_FAIL;
	}

	/*if (FAILED(m_pGameInstance->Add_Clone(LEVEL_GAMEPLAY, strLayerTag, G0_OBJECTMESH_DEMO_TAG,
		&AnimMeshDemoValue)))
		return E_FAIL;*/
	
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

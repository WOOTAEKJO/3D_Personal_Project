#include "stdafx.h"
#include "..\Public\DynamicCamera.h"
#include "GameInstance.h"

CDynamicCamera::CDynamicCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CDynamicCamera::CDynamicCamera(const CDynamicCamera& rhs)
	:CCamera(rhs)
{
}

HRESULT CDynamicCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDynamicCamera::Initialize(void* pArg)
{
	CDynamicCamera::DYNAMICCAMERADESC DynamicCameraDesc;

	DynamicCameraDesc.fMouseSensitivity = 0.1f;
	DynamicCameraDesc.vEye = _float4(0.f, 5.f, -3.f, 1.f);
	//DynamicCameraDesc.vEye = _float4(44.f, 12.f, 33.f, 1.f);
	DynamicCameraDesc.vAte = _float4(0.f, 0.f, 0.f, 1.f);
	DynamicCameraDesc.fFovy = XMConvertToRadians(60.f);
	DynamicCameraDesc.fAspect = ((_float)g_iWinSizeX) / g_iWinSizeY;
	DynamicCameraDesc.fNear = 0.1f;
	DynamicCameraDesc.fFar = 1000.f;
	DynamicCameraDesc.fSpeedPerSec = 5.f;
	DynamicCameraDesc.fRotationPerSec = XMConvertToRadians(180.f);

	if (FAILED(__super::Initialize(&DynamicCameraDesc)))
		return E_FAIL;

	m_fMouseSensitivity = 0.1f;

	return S_OK;
}

void CDynamicCamera::Priority_Tick(_float fTimeDelta)
{
}

void CDynamicCamera::Tick(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
	
	m_pGameInstance->Free_Mouse(fTimeDelta,m_fMouseSensitivity,m_pTransformCom);

	__super::Tick(fTimeDelta);
}

void CDynamicCamera::Late_Tick(_float fTimeDelta)
{
}

void CDynamicCamera::Key_Input(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(DIK_S))
	{
		m_pTransformCom->Go_BackWard(fTimeDelta);
	}
}

CDynamicCamera* CDynamicCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamicCamera* pInstance = new CDynamicCamera(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamicCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CDynamicCamera::Clone(void* pArg)
{
	CDynamicCamera* pInstance = new CDynamicCamera(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDynamicCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDynamicCamera::Free()
{
	__super::Free();
}

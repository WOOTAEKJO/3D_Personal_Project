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
	if (pArg == nullptr)
		return E_FAIL;

	DYNAMICCAMERADESC* pDynamicCameraDesc = (DYNAMICCAMERADESC*)pArg;

	if (FAILED(__super::Initialize(pDynamicCameraDesc)))
		return E_FAIL;

	m_fMouseSensitivity = pDynamicCameraDesc->fMouseSensitivity;

	return S_OK;
}

void CDynamicCamera::Priority_Tick(_float fTimeDelta)
{
}

void CDynamicCamera::Tick(_float fTimeDelta)
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_BackWard(fTimeDelta);
	}

	__super::Tick(fTimeDelta);
}

void CDynamicCamera::Late_Tick(_float fTimeDelta)
{
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

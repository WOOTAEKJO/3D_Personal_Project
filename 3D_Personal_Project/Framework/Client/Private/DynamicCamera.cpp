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
	Key_Input(fTimeDelta);
	
	Mouse_Move(fTimeDelta);
	//Mouse_Fix();

	__super::Tick(fTimeDelta);
}

void CDynamicCamera::Late_Tick(_float fTimeDelta)
{
}

void CDynamicCamera::Mouse_Fix()
{
	POINT	pt{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CDynamicCamera::Mouse_Move(_float fTimeDelta)
{
	_long MouseMove = 0;

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensitivity * MouseMove * fTimeDelta);
	}

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE::STATE_RIGHT),
			m_fMouseSensitivity * MouseMove * fTimeDelta);
	}
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

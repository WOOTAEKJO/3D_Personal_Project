#include "stdafx.h"
#include "..\Public\TargetCamera.h"
#include "GameInstance.h"

#include "Player.h"

CTargetCamera::CTargetCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CTargetCamera::CTargetCamera(const CTargetCamera& rhs)
	:CCamera(rhs)
{
}

HRESULT CTargetCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTargetCamera::Initialize(void* pArg)
{
	/*if (pArg == nullptr)
		return E_FAIL;*/

	m_pTarget = m_pGameInstance->Get_ObjectList(LEVEL_GAMEPLAY, TEXT("Player")).front();
	if (m_pTarget == nullptr)
		return E_FAIL;
	Safe_AddRef(m_pTarget);

	CAMERADESC CameraDesc = {};

	m_pTargetTransform = m_pTarget->Get_Component<CTransform>();
	if (m_pTargetTransform == nullptr)
		return E_FAIL;
	//Safe_AddRef(m_pTargetTransform);

	m_pTargetStateMachine = m_pTarget->Get_Component<CStateMachine>();
	if (m_pTargetStateMachine == nullptr)
		return E_FAIL;

	_float4 vPos,vDir;
	XMStoreFloat4(&vPos, m_pTargetTransform->Get_State(CTransform::STATE::STATE_POS));
	XMStoreFloat4(&vDir, XMVector4Normalize(m_pTargetTransform->Get_State(CTransform::STATE::STATE_LOOK)));

	CameraDesc.vEye = _float4(vPos.x - (vDir.x) * m_iRadiusX, vPos.y + m_iRadiusY, vPos.z - (vDir.z) * m_iRadiusX, vPos.w);
	CameraDesc.vAte = vPos;
	CameraDesc.fFovy = XMConvertToRadians(60.f);
	CameraDesc.fAspect = ((_float)g_iWinSizeX) / g_iWinSizeY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 30.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(180.f);

	if (FAILED(__super::Initialize(&CameraDesc)))
		return E_FAIL;

	return S_OK;
}

void CTargetCamera::Priority_Tick(_float fTimeDelta)
{
	//Mouse_Fix();
	Mouse_Input(fTimeDelta);
}

void CTargetCamera::Tick(_float fTimeDelta)
{ 	
	__super::Tick(fTimeDelta);
}

void CTargetCamera::Late_Tick(_float fTimeDelta)
{

}

void CTargetCamera::Mouse_Input(_float fTimeDelta)
{
	_bool bCheck = true;

	if (m_pTargetStateMachine->Get_StateID() == (_uint)CPlayer::STATE::IDLE)
		bCheck = false;

	_long MouseMove = 0;

	_vector vTargetPos, vPos, vDir, vEye;

	_matrix matRotY = XMMatrixIdentity();
	_matrix matRotX = XMMatrixIdentity();

	vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE::STATE_POS);
	vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vDir = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK));

	if(!bCheck)
	{
		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
		{
			_float fRadian = MouseMove * fTimeDelta * 0.5f;
			_float fDegree = XMConvertToDegrees(fRadian);

			if (fDegree<90.f && fDegree > -90.f) {

			}
			matRotX = XMMatrixRotationX(fRadian);
		}
	}

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		_float fRadian = MouseMove * fTimeDelta * 0.5f;

		matRotY = XMMatrixRotationY(fRadian);

	}

	vDir = (XMVector3TransformNormal(vDir, matRotX * matRotY)) * m_iRadiusX;

	vEye = XMVectorSet(vTargetPos.m128_f32[0] - (vDir.m128_f32[0]),
		bCheck == true ? vTargetPos.m128_f32[1] + (_float)m_iRadiusY : vTargetPos.m128_f32[1] - vDir.m128_f32[1],
		vTargetPos.m128_f32[2] - (vDir.m128_f32[2]),
		1.f);

	if(bCheck)
	{
		vDir = (vEye - vPos);
		vPos = XMVectorSet(vPos.m128_f32[0] + vDir.m128_f32[0] * m_iRadiusX,
			vPos.m128_f32[1] + vDir.m128_f32[1] * fTimeDelta * 3.f,
			vPos.m128_f32[2] + vDir.m128_f32[2] * m_iRadiusX, 1.f);
	}

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, bCheck ? vPos : vEye);
	m_pTransformCom->LookAt(vTargetPos);
}

void CTargetCamera::Mouse_Fix()
{
	POINT	pt{ (_long)g_iWinSizeX >> 1, (_long)g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CTargetCamera::Target_Follow(_float fTimeDelta)
{
	_vector vTargetPos, vPos, vDir, vEye;
	vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE::STATE_POS);
	vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vDir = m_pTargetTransform->Get_State(CTransform::STATE::STATE_LOOK);
	
	vDir = XMVector3Normalize(vDir) * m_iRadiusX;

	//vDir = XMVector3Normalize(vTargetPos - vPos);

	vEye = XMVectorSet((vTargetPos.m128_f32[0] - (vDir.m128_f32[0])),
		(vTargetPos.m128_f32[1] + (_float)m_iRadiusY),
		(vTargetPos.m128_f32[2] - (vDir.m128_f32[2])),
		1.f);

	vDir = (vEye - vPos);

	vPos += vDir * fTimeDelta * 5.f ;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	m_pTransformCom->LookAt(vTargetPos);
}

CTargetCamera* CTargetCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTargetCamera* pInstance = new CTargetCamera(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTargetCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTargetCamera::Clone(void* pArg)
{
	CTargetCamera* pInstance = new CTargetCamera(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTargetCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTargetCamera::Free()
{
	//Safe_Release(m_pTransformCom);
	Safe_Release(m_pTarget);

	__super::Free();

}

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

	m_pTarget = m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER::LAYER_PLAYER]).front();
	if (m_pTarget == nullptr)
		return E_FAIL;
	//Safe_AddRef(m_pTarget);

	m_pOriginTarget = m_pTarget;

	CAMERADESC CameraDesc = {};

	m_pTargetTransform = m_pTarget->Get_Component<CTransform>();
	if (m_pTargetTransform == nullptr)
		return E_FAIL;
	//Safe_AddRef(m_pTargetTransform);

	m_pTargetStateMachine = m_pTarget->Get_Component<CStateMachine>();
	if (m_pTargetStateMachine == nullptr)
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vOffset = m_vOriginOffset;
	m_fLookAt_Height = m_fOriginLookAt_Height;

	if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_BOSS2)
		m_vOffset = _float3(-1.5f, -0.5f, -1.5f);

	_vector vPos,vDir;
	vPos = m_pTargetTransform->Get_State(CTransform::STATE::STATE_POS);
	vDir = XMVector4Normalize(m_pTargetTransform->Get_State(CTransform::STATE::STATE_LOOK));

	XMStoreFloat4(&CameraDesc.vEye, vPos - XMLoadFloat3(&m_vOffset));
	XMStoreFloat4(&CameraDesc.vAte, vPos);
	CameraDesc.fFovy = XMConvertToRadians(60.f);
	CameraDesc.fAspect = ((_float)g_iWinSizeX) / g_iWinSizeY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 30.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(180.f);

	XMStoreFloat4(&m_vPrevTargetPos, vPos);

	if (FAILED(__super::Initialize(&CameraDesc)))
		return E_FAIL;

	m_fSpringConstant = 700.f;
	m_fDampConstant = 2.f * sqrt(m_fSpringConstant);
	m_vVeclocity = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

void CTargetCamera::Priority_Tick(_float fTimeDelta)
{
	//Mouse_Fix();
	StateTrans(fTimeDelta);
	if (m_bCutScene)
		CutScene(fTimeDelta);
	else
		Mouse_Input(fTimeDelta);
}

void CTargetCamera::Tick(_float fTimeDelta)
{ 	
	__super::Tick(fTimeDelta);
}

void CTargetCamera::Late_Tick(_float fTimeDelta)
{

}

_vector CTargetCamera::Camera_Shaking(_fvector vCamPos, _float fTimeDelta)
{
	m_fShakingTimeAcc += fTimeDelta;

	_vector vCurCamPos = vCamPos;

	if (m_fShakingTimeAcc < m_fShakingTime)
	{
		_float fX = m_fShakingAmplitude *
			cosf(m_fShakingTimeAcc * m_fShakingSpeed +
				(((_float)rand() / (_float)RAND_MAX) * XMConvertToRadians(180.f)));
		_float fY = m_fShakingAmplitude *
			-sinf(m_fShakingTimeAcc * m_fShakingSpeed +
				(((_float)rand() / (_float)RAND_MAX) * XMConvertToRadians(180.f)));

		_vector vShakePos = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)) * fX +
			XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP)) * fY * 0.5f;

		vCurCamPos += vShakePos;
	}
	else {
		m_bShaking = false;
	}

	return vCurCamPos;
}

void CTargetCamera::Shaking_SetUp(_float fAmplitude, _float fSpeed, _float fTime)
{
	m_bShaking = true;
	m_fShakingAmplitude = fAmplitude;
	m_fShakingSpeed = fSpeed;
	m_fShakingTimeAcc = 0.f;
	m_fShakingTime = fTime;
}

void CTargetCamera::Change_Target(_float fSensitivity, _bool bCutScene, CGameObject* pTarget)
{
	m_bCutScene = bCutScene;

	if (pTarget == nullptr)
	{
		//m_bPlayer = true;
		m_pTarget = m_pOriginTarget;
	}
	else {
		//m_bPlayer = false;
		m_pTarget = pTarget;
	}

	m_pTargetTransform = m_pTarget->Get_Component<CTransform>();

	m_fSensitivity = fSensitivity;
}

HRESULT CTargetCamera::Ready_Component()
{
	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = m_pTarget->Get_Component<CNavigation>()->Get_CurrentIndex();
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom, &NavigationDesc))) return E_FAIL;

	return S_OK;
}

void CTargetCamera::StateTrans(_float fTimeDelta)
{
	if (m_bCutScene)
		return;

	if (m_pTargetStateMachine->Get_StateID() == (_uint)CPlayer::STATE::IDLE)
	{
		m_fTransAcc += fTimeDelta;

		if (m_fTransAcc > 0.1f)
			m_bStateTrans = false;
	}
	else
	{
		m_bStateTrans = true;
		m_fTransAcc = 0.f;
	}
		
}

void CTargetCamera::Mouse_Input(_float fTimeDelta)
{
	_long MouseMove = 0;

	_vector vTargetPos, vPos, vDir, vEye;

	_matrix matRot = XMMatrixIdentity();

	vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE::STATE_POS);
	vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	vTargetPos.m128_f32[1] += m_pTargetTransform->Get_Scaled().y;

	if (!m_bStateTrans)
	{
		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
		{
			m_fAngleAccY += MouseMove * fTimeDelta * 0.5f;

			m_fAngleAccY = Clamp(XMConvertToRadians(-30.f), XMConvertToRadians(30.f), m_fAngleAccY);
		}
	}

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		m_fAngleAccX += MouseMove * fTimeDelta * 0.5f;
	}

	_vector vRot = XMQuaternionRotationRollPitchYaw(m_fAngleAccY, m_fAngleAccX, 0.f);
	matRot = XMMatrixRotationQuaternion(vRot);

	vDir = (XMVector3TransformNormal(XMLoadFloat3(&m_vOffset), matRot));

	vEye = vTargetPos - vDir;

	if (m_bStateTrans)
		vEye.m128_f32[1] = vTargetPos.m128_f32[1] - m_vOffset.y;

	if (m_bShaking)
	{
		vEye = Camera_Shaking(vEye, fTimeDelta);
	}

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, Camera_Spring(vEye, vPos, fTimeDelta * m_fSensitivity));

	vTargetPos.m128_f32[1] += m_pTargetTransform->Get_Scaled().y * m_fLookAt_Height;
	m_vPrevTargetPos.y = vTargetPos.m128_f32[1];

	m_pTransformCom->LookAt(Camera_Spring(XMLoadFloat4(&m_vPrevTargetPos), vTargetPos, fTimeDelta * m_fSensitivity));

	XMStoreFloat4(&m_vPrevTargetPos, vTargetPos);
}

void CTargetCamera::Mouse_Fix()
{
	POINT	pt{ (_long)g_iWinSizeX >> 1, (_long)g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CTargetCamera::CutScene(_float fTimeDelta)
{
	_long MouseMove = 0;

	_vector vTargetPos, vPos, vDir, vEye;

	_matrix matRot = XMMatrixIdentity();

	vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE::STATE_POS);
	vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	vDir = XMLoadFloat3(&m_vOffset);

	vEye = vTargetPos - vDir;

	vTargetPos.m128_f32[1] += m_pTargetTransform->Get_Scaled().y * m_fLookAt_Height;

	if (m_bCutSceneSpring)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, Camera_Spring(vEye, vPos, fTimeDelta * m_fSensitivity));
		m_pTransformCom->LookAt(Camera_Spring(XMLoadFloat4(&m_vPrevTargetPos), vTargetPos, fTimeDelta * m_fSensitivity));

		XMStoreFloat4(&m_vPrevTargetPos, vTargetPos);
	}
	else {
		

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vEye);
		m_pTransformCom->LookAt(vTargetPos);
	}
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
	//Safe_Release(m_pTarget);
	Safe_Release(m_pNavigationCom);

	__super::Free();

}

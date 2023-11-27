#include "..\Public\Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CCamera::CCamera(const CCamera& rhs)
	:CGameObject(rhs),m_fFovy(rhs.m_fFovy),m_fAspect(rhs.m_fAspect),
	m_fNear(rhs.m_fNear),m_fFar(rhs.m_fFar)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	CAMERADESC* pCameraDesc = (CAMERADESC*)pArg;

	if (FAILED(__super::Initialize(pCameraDesc)))
		return E_FAIL;

	m_fFovy = pCameraDesc->fFovy;
	m_fAspect = pCameraDesc->fAspect;
	m_fNear = pCameraDesc->fNear;
	m_fFar = pCameraDesc->fFar;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, pCameraDesc->vEye);
	m_pTransformCom->LookAt(XMLoadFloat4(&pCameraDesc->vAte));

	return S_OK;
}

void CCamera::Priority_Tick(_float fTimeDelta)
{
}

void CCamera::Tick(_float fTimeDelta)
{
	m_pGameInstance->Set_Transform(CPipeLine::TRANSFORMSTATE::VIEW, 
		XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix_Matrix()));
	m_pGameInstance->Set_Transform(CPipeLine::TRANSFORMSTATE::PROJ,
		XMMatrixPerspectiveFovLH(m_fFovy,m_fAspect,m_fNear,m_fFar));
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}

void CCamera::Free()
{
	__super::Free();
}

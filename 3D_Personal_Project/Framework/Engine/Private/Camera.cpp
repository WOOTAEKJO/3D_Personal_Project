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

void CCamera::Camera_Sliding(_fvector vPosition, CNavigation* pNavigation, _float fTimeDelta)
{
	if (pNavigation == nullptr || m_pTransformCom == nullptr)
		return;

	_float3 vLine = {};
	_vector vSlidePos;
	_vector vResultPos = vPosition;

	m_bWallCheck = false;

	if (!pNavigation->IsMove(vPosition, &vLine))
	{
		m_bWallCheck = true;

		vSlidePos = m_pTransformCom->Sliding(vLine, fTimeDelta * 2.f);

		vSlidePos.m128_f32[1] = 0.f;

		vResultPos += vSlidePos;

		if (!pNavigation->IsMove(vResultPos, &vLine))
			return;
	}

	/*_float3 vPos;
	XMStoreFloat3(&vPos, vResultPos);

	_float fHeigh = pNavigation->Get_Cell_Height(vPos);

	if (fHeigh + 0.2f >= vPos.y)
	{
		m_bWallCheck = true;
		vResultPos.m128_f32[1] = fHeigh + 0.3f;
	}*/

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vResultPos);

}

_vector CCamera::Camera_Spring(_fvector vEye, _float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	_vector vDisplacement = vPos - vEye;

	_vector vSpringAccel = (-m_fSpringConstant * vDisplacement) - (m_fDampConstant * XMLoadFloat3(&m_vVeclocity));

	XMStoreFloat3(&m_vVeclocity, XMLoadFloat3(&m_vVeclocity) + vSpringAccel * fTimeDelta);

	XMStoreFloat3(&m_vActualPosition, vPos + XMLoadFloat3(&m_vVeclocity) * fTimeDelta);

	return XMVectorSetW(XMLoadFloat3(&m_vActualPosition),1.f);
}

void CCamera::Free()
{
	__super::Free();
}

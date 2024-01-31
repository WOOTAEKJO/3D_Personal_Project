#include "..\Public\Camera.h"
#include "GameInstance.h"
#include "Cell.h"
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
	_matrix matView = XMMatrixInverse(nullptr, m_pTransformCom->Get_WorldMatrix_Matrix());
	_matrix matProj = XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar);

	m_pGameInstance->Set_Transform(CPipeLine::TRANSFORMSTATE::VIEW, matView);
	m_pGameInstance->Set_Transform(CPipeLine::TRANSFORMSTATE::PROJ, matProj);

	/*_float4x4 View, Proj;

	XMStoreFloat4x4(&View, matView);
	XMStoreFloat4x4(&Proj, matProj);

	m_pGameInstance->Get_ShadowLight()->Set_Matrix(CShadowLight::STATE::VIEW, View);
	m_pGameInstance->Get_ShadowLight()->Set_Matrix(CShadowLight::STATE::PROJ, Proj);*/
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

	if (pNavigation->Get_Navigation_Cells()[pNavigation->Get_CurrentIndex()]->Get_CellType() != CCell::CELLTYPE::TYPE_JUMP)
	{
		if (!pNavigation->IsMove(vPosition, &vLine))
		{
			m_bWallCheck = true;

			_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

			_vector vDir = vPosition - vPos;

			//vLine = _float3(-vLine.x, vLine.y, -vLine.z);

			vSlidePos = m_pTransformCom->Sliding(vDir,vLine, fTimeDelta);

			vSlidePos.m128_f32[1] = 0.f;

			vResultPos = vPos + vSlidePos;

			if (!pNavigation->IsMove(vResultPos, &vLine))
				return;
			else
				int a = 0;
		}
	}

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vResultPos);

}

_vector CCamera::Camera_Spring(_fvector vTargetPos, _fvector vPos, _float fTimeDelta, _float fRatio)
{
	_vector vResultPos;

	_vector vDisplacement = vPos - vTargetPos;

	_vector vSpringAccel = (-m_fSpringConstant * vDisplacement) - (m_fDampConstant * XMLoadFloat3(&m_vVeclocity));

	XMStoreFloat3(&m_vVeclocity, XMLoadFloat3(&m_vVeclocity) + vSpringAccel * fTimeDelta);

	vResultPos = vPos + XMLoadFloat3(&m_vVeclocity) * fTimeDelta;

	//vResultPos = XMVectorLerp(vPos, vResultPos, fRatio);

	return vResultPos;
}

void CCamera::Free()
{
	__super::Free();
}

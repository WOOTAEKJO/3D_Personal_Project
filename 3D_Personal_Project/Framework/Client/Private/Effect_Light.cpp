#include "stdafx.h"
#include "..\Public\Effect_Light.h"

#include "GameInstance.h"
#include "Character.h"

#include "Bone.h"

CEffect_Light::CEffect_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Light::CEffect_Light(const CEffect_Light& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Light::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Light::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_Light_Info = *(EFFECT_LIGHT_INFO*)pArg;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(m_vSize.x, m_vSize.y, 0.1f);

	m_pOwnerTransform = m_pOwner->Get_Component<CTransform>();
	if (m_pOwnerTransform == nullptr)
		return E_FAIL;

	m_pSocketBone = m_Light_Info.pSocketBone;
	if (m_pSocketBone == nullptr)
		return E_FAIL;

	//XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() * m_pSocketBone->Get_CombinedTransformationMatrix() * m_pOwnerTransform->Get_WorldMatrix_Matrix());

	return S_OK;
}

void CEffect_Light::Priority_Tick(_float fTimeDelta)
{

	XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() * m_pSocketBone->Get_CombinedTransformationMatrix() * m_pOwnerTransform->Get_WorldMatrix_Matrix());
	Look_Cam();
	/*__super::Tick(fTimeDelta);
	Update_Pos();*/
}

void CEffect_Light::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEffect_Light::Late_Tick(_float fTimeDelta)
{
	if (m_pOwner == nullptr || m_pOwner->Get_Dead())
		Set_Dead();

	if (m_bActivate)
	{
		_float4 vPos;
		memcpy(&vPos, &m_matWorldMat.m[3], sizeof(_float4));

		_vector vTmpPos = XMLoadFloat4(&vPos);
		_vector vCamPos = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_POS);

		if (XMVector3NearEqual(vTmpPos, vCamPos, XMVectorSet(9.f, 9.f, 9.f, 0.f)))
			__super::Late_Tick(fTimeDelta);
	}
}

HRESULT CEffect_Light::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(12);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Light::Bind_ShaderResources()
{

	/*if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;*/
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorldMat)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_DEPTH, m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_Light_Info.vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Light::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	if (FAILED(Add_Component<CTexture>(m_Light_Info.strMaskTag, &m_pMaskTextureCom,nullptr,1))) return E_FAIL;

	return S_OK;
}

void CEffect_Light::Update_Pos()
{
	if (m_pOwner == nullptr)
		return;

	_vector vPos = m_pOwnerTransform->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_f32[1] += m_pOwnerTransform->Get_Scaled().y;

	/*_vector vCamPos = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_POS);

	_vector vDir = XMVector3Normalize(vPos - vCamPos);

	vPos += vDir * 1.f;*/

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
}

void CEffect_Light::Look_Cam()
{
	_vector vCamPos = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_POS);

	_float fX = XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_matWorldMat).r[0]));
	_float fY = XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_matWorldMat).r[1]));
	_float fZ = XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_matWorldMat).r[2]));

	_vector	vPos = XMLoadFloat4x4(&m_matWorldMat).r[3];

	_vector vLook = XMVector3Normalize(vCamPos - vPos) * fZ;
	_vector	vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * fX;
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * fY;

	_vector vDir = XMVector3Normalize(vPos - vCamPos);

	vPos += vDir * 0.1f;

	_float4 vLookTmp, vRightTmp, vUpTmp,vPosTmp;
	XMStoreFloat4(&vLookTmp, XMVectorSetW(vLook, 0.f));
	XMStoreFloat4(&vRightTmp, XMVectorSetW(vRight, 0.f));
	XMStoreFloat4(&vUpTmp, XMVectorSetW(vUp, 0.f));
	XMStoreFloat4(&vPosTmp, XMVectorSetW(vPos, 1.f));

	memcpy(&m_matWorldMat.m[0], &vRightTmp, sizeof(_float4));
	memcpy(&m_matWorldMat.m[1], &vUpTmp, sizeof(_float4));
	memcpy(&m_matWorldMat.m[2], &vLookTmp, sizeof(_float4));
	memcpy(&m_matWorldMat.m[3], &vPos, sizeof(_float4));

}

CEffect_Light* CEffect_Light::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Light* pInstance = new CEffect_Light(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Light");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Light::Clone(void* pArg)
{
	CEffect_Light* pInstance = new CEffect_Light(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Light");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Light::Free()
{
	__super::Free();

	Safe_Release(m_pMaskTextureCom);

}

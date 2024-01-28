#include "stdafx.h"
#include "..\Public\Effect_Target.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_Target::CEffect_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Target::CEffect_Target(const CEffect_Target& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Target::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Target::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	EFFECT_TARGETINFO* Info = (EFFECT_TARGETINFO*)pArg;

	m_vSolid_Color = Info->vColor;
	m_fRadius = Info->fRadius;
	m_fCompareRadius = Info->fCompareRadius;

	m_fMaxFrame = m_pTextureCom->Get_TextureNum();

	if (m_fMaxFrame > 1)
	{
		m_bSprite = true;
		m_fFrame = (_float)(rand() % (_uint)m_fMaxFrame);

	}
	m_pTransformCom->Rotation_Quaternio(XMConvertToRadians(90.f), 0.f, 0.f);
	m_pTransformCom->Set_Scaling(m_vSize.x, m_vSize.y, 0.1f);

	m_fAlpha = 0.6f;

	_float3 fScale = m_pTransformCom->Get_Scaled();

	m_fRadius = (fScale.x * 0.4f);
	m_fCompareRadius = (fScale.x * 0.5f);

	return S_OK;
}

void CEffect_Target::Priority_Tick(_float fTimeDelta)
{

	//__super::Priority_Tick(fTimeDelta);
}

void CEffect_Target::Tick(_float fTimeDelta)
{
	//m_fRadius += fTimeDelta;
	__super::Tick(fTimeDelta);
}

void CEffect_Target::Late_Tick(_float fTimeDelta)
{
	//Judge_Dead(fTimeDelta);

	__super::Late_Tick(fTimeDelta);

	if (m_pOwner == nullptr || m_pOwner->Get_Dead())
		Set_Dead();
}

HRESULT CEffect_Target::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(7);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Target::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCenter", &m_vPos, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRadius", &m_fRadius, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fCompare_Radius", &m_fCompareRadius, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Target::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

CEffect_Target* CEffect_Target::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Target* pInstance = new CEffect_Target(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Target");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Target::Clone(void* pArg)
{
	CEffect_Target* pInstance = new CEffect_Target(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Target");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Target::Free()
{
	__super::Free();

}

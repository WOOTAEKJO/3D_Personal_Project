#include "stdafx.h"
#include "..\Public\Effect_Damage.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_Damage::CEffect_Damage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Damage::CEffect_Damage(const CEffect_Damage& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Damage::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Damage::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fMaxFrame = m_pTextureCom->Get_TextureNum();

	if (m_fMaxFrame > 1)
	{
		m_bSprite = true;
		m_fFrame = (_float)(rand() % (_uint)m_fMaxFrame);

	}

	m_pTransformCom->Set_Scaling(m_vSize.x, m_vSize.y, 0.1f);

	m_vSolid_Color = { 1.f,1.f,1.f,1.f };

	return S_OK;
}

void CEffect_Damage::Priority_Tick(_float fTimeDelta)
{

	__super::Priority_Tick(fTimeDelta);
}

void CEffect_Damage::Tick(_float fTimeDelta)
{
	Size_Up(fTimeDelta);
	__super::Tick(fTimeDelta);
}

void CEffect_Damage::Late_Tick(_float fTimeDelta)
{
	Judge_Dead(fTimeDelta);

	__super::Late_Tick(fTimeDelta);

}

HRESULT CEffect_Damage::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Damage::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Damage::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

CEffect_Damage* CEffect_Damage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Damage* pInstance = new CEffect_Damage(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Damage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Damage::Clone(void* pArg)
{
	CEffect_Damage* pInstance = new CEffect_Damage(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Damage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Damage::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\Effect_Halo.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_Halo::CEffect_Halo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Halo::CEffect_Halo(const CEffect_Halo& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Halo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Halo::Initialize(void* pArg)
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

	m_pTransformCom->Set_Scaling(0.5f, 0.5f, 0.5f);

	

	return S_OK;
}

void CEffect_Halo::Priority_Tick(_float fTimeDelta)
{

	__super::Priority_Tick(fTimeDelta);
}

void CEffect_Halo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEffect_Halo::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	/*if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
		return;*/

}

HRESULT CEffect_Halo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Halo::Bind_ShaderResources()
{
	/*if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;*/

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	/*if (FAILED(m_pMaskTexture->Bind_ShaderResources(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CEffect_Halo::Ready_Component()
{
	

	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	//if (FAILED(Add_Component<CTexture>(MASK_GLOWTEST_TAG, &m_pMaskTexture,nullptr,1))) return E_FAIL;

	return S_OK;
}

CEffect_Halo* CEffect_Halo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Halo* pInstance = new CEffect_Halo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Halo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Halo::Clone(void* pArg)
{
	CEffect_Halo* pInstance = new CEffect_Halo(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Halo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Halo::Free()
{
	__super::Free();

}

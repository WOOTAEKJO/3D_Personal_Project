#include "stdafx.h"
#include "..\Public\Effect_Water.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_Water::CEffect_Water(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Water::CEffect_Water(const CEffect_Water& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Water::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Water::Initialize(void* pArg)
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
	m_pTransformCom->Rotation_Quaternio(XMConvertToRadians(90.f), 0.f, 0.f);

	m_fAlpha = 0.5f;

	return S_OK;
}

void CEffect_Water::Priority_Tick(_float fTimeDelta)
{

	//__super::Priority_Tick(fTimeDelta);
}

void CEffect_Water::Tick(_float fTimeDelta)
{
	//Size_Up(fTimeDelta);
	__super::Tick(fTimeDelta);
}

void CEffect_Water::Late_Tick(_float fTimeDelta)
{
	//Judge_Dead(fTimeDelta);

	m_fTimeDelta += fTimeDelta;

	__super::Late_Tick(fTimeDelta);

}

HRESULT CEffect_Water::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(8);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Water::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Water::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

CEffect_Water* CEffect_Water::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Water* pInstance = new CEffect_Water(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Water");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Water::Clone(void* pArg)
{
	CEffect_Water* pInstance = new CEffect_Water(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Water");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Water::Free()
{
	__super::Free();

}

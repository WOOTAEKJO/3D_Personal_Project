#include "stdafx.h"
#include "..\Public\UI_HP.h"

#include "GameInstance.h"

#include "Character.h"

CUI_HP::CUI_HP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CUI_HP::CUI_HP(const CUI_HP& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_HP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_HP::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UI_HP_DESC* pDesc = (UI_HP_DESC*)pArg;

	m_pOwner = pDesc->pOwner;
	if (m_pOwner == nullptr)
		return E_FAIL;

	m_pMaskTag = pDesc->strMaskTexture;
	m_vSolidColor = pDesc->vSolidColor;
	m_iShaderPassIndx = pDesc->iShaderPassIndx;
	m_strNoiseTexture = pDesc->strNoiseTexture;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vHPColor = _float4(0.6f, 0.2f, 0.2f, 0.3f);

	return S_OK;
}

void CUI_HP::Priority_Tick(_float fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;
}

void CUI_HP::Tick(_float fTimeDelta)
{
	Compute_Ratio();
}

void CUI_HP::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CUI_HP::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iShaderPassIndx);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();
	return S_OK;
}

HRESULT CUI_HP::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pMaskTexture->Bind_ShaderResources(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;

	if (FAILED(m_pNoiseTexture->Bind_ShaderResources(m_pShaderCom, "g_NoiseTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolidColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRadius", &m_fHP_Ratio, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HP::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_pMaskTag, &m_pMaskTexture,nullptr,1))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_strNoiseTexture, &m_pNoiseTexture, nullptr, 2))) return E_FAIL;
	
	return S_OK;
}

void CUI_HP::Compute_Ratio()
{
	CCharacter* pOwner = dynamic_cast<CCharacter*>(m_pOwner);

	_float iMaxHP = pOwner->Open_Status_Desc()->iMaxHP;
	_float iCurHP = pOwner->Open_Status_Desc()->iCurHP;

	m_fHP_Ratio = iCurHP / iMaxHP;
}

CUI_HP* CUI_HP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HP* pInstance = new CUI_HP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_HP");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_HP::Clone(void* pArg)
{
	CUI_HP* pInstance = new CUI_HP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_HP::Free()
{
	__super::Free();

	Safe_Release(m_pMaskTexture);
	Safe_Release(m_pNoiseTexture);

}

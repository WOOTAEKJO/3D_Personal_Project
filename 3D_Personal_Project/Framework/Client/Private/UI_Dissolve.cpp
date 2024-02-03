#include "stdafx.h"
#include "..\Public\UI_Dissolve.h"

#include "GameInstance.h"

CUI_Dissolve::CUI_Dissolve(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CUI_Dissolve::CUI_Dissolve(const CUI_Dissolve& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Dissolve::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dissolve::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_Dissolve_Desc = *((UI_DISSOLVE_DESC*)pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_Dissolve_Desc.fDissolveAmount = 1.f;
	m_Dissolve_Desc.fDissolveGradiationDistance = 1.f;
	m_Dissolve_Desc.vDissolveGradiationStartColor = _float3(0.f, 0.f, 0.f);
	m_Dissolve_Desc.vDissolveGradiationGoalColor = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

void CUI_Dissolve::Priority_Tick(_float fTimeDelta)
{
	m_fTimeDelta += 0.5f;
}

void CUI_Dissolve::Tick(_float fTimeDelta)
{
	Dissolve_Render(fTimeDelta);

	/*if (m_pGameInstance->Key_Down(DIK_6))
	{
		if (m_Dissolve_Desc.fDissolveAmount < 1.f)
			m_Dissolve_Desc.fDissolveAmount += 0.1f;
	}

	if (m_pGameInstance->Key_Down(DIK_7))
	{
		if (m_Dissolve_Desc.fDissolveAmount > 0.f)
			m_Dissolve_Desc.fDissolveAmount -= 0.1f;
	}
	
	if (m_pGameInstance->Key_Down(DIK_8))
	{
		if (m_Dissolve_Desc.fDissolveGradiationDistance < 1.f)
			m_Dissolve_Desc.fDissolveGradiationDistance += 0.1f;
	}

	if (m_pGameInstance->Key_Down(DIK_9))
	{
		if (m_Dissolve_Desc.fDissolveGradiationDistance > 0.f)
			m_Dissolve_Desc.fDissolveGradiationDistance -= 0.1f;
	}*/
}

void CUI_Dissolve::Late_Tick(_float fTimeDelta)
{
	if (m_bTrueRender)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return;
	}
}

HRESULT CUI_Dissolve::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(11);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();
	return S_OK;
}

HRESULT CUI_Dissolve::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pDissolveTexture->Bind_ShaderResources(m_pShaderCom, "g_DissolveTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &m_Dissolve_Desc.fDissolveAmount, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveGradiationDistance", &m_Dissolve_Desc.fDissolveGradiationDistance, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveGradiationStartColor", &m_Dissolve_Desc.vDissolveGradiationStartColor, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveGradiationGoalColor", &m_Dissolve_Desc.vDissolveGradiationGoalColor, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Dissolve::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_Dissolve_Desc.strDissolveTexture, &m_pDissolveTexture, nullptr, 1))) return E_FAIL;

	return S_OK;
}

void CUI_Dissolve::Dissolve_Render(_float fTimeDelta)
{
	if (m_bRender)
	{
		if (!m_bTrueRender)
			m_bTrueRender = true;

		if (m_Dissolve_Desc.fDissolveAmount > 0.f)
			m_Dissolve_Desc.fDissolveAmount -= fTimeDelta;

		if (m_Dissolve_Desc.fDissolveGradiationDistance > 0.f)
			m_Dissolve_Desc.fDissolveGradiationDistance -= fTimeDelta * 1.5f; 

		if (m_Dissolve_Desc.fDissolveAmount <= 0.f)
			m_bReady = true;
	}
	else
	{
		m_bReady = false;

		if (m_Dissolve_Desc.fDissolveAmount < 1.f)
			m_Dissolve_Desc.fDissolveAmount += fTimeDelta * 1.5f;

		if (m_Dissolve_Desc.fDissolveGradiationDistance < 1.f)
			m_Dissolve_Desc.fDissolveGradiationDistance += fTimeDelta ;

		if(m_Dissolve_Desc.fDissolveGradiationDistance >= 1.f)
			m_bTrueRender = false;
	}
}

CUI_Dissolve* CUI_Dissolve::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dissolve* pInstance = new CUI_Dissolve(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Dissolve");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Dissolve::Clone(void* pArg)
{
	CUI_Dissolve* pInstance = new CUI_Dissolve(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Dissolve::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTexture);

}

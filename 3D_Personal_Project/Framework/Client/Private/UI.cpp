#include "stdafx.h"
#include "..\Public\UI.h"

#include "GameInstance.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CUI::CUI(const CUI& rhs)
	:CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tUI_Desc = *(UI_DESC*)pArg;

	m_pTransformCom->Set_Scaling(m_tUI_Desc.vScale.x, m_tUI_Desc.vScale.y, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POS, 
		XMVectorSet(m_tUI_Desc.vCenterPos.x - g_iWinSizeX * 0.5f, -m_tUI_Desc.vCenterPos.y +
			g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI::Priority_Tick(_float fTimeDelta)
{
}

void CUI::Tick(_float fTimeDelta)
{
	
}

void CUI::Late_Tick(_float fTimeDelta)
{
	if (m_bRender)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return;
	}

}

HRESULT CUI::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CUI::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_POS_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_tUI_Desc.strTextureTag, &m_pTextureCom))) return E_FAIL;
	if (FAILED(Add_Component<CVIBuffer_DRect>(BUFFER_DRECT_TAG, &m_pBufferCom))) return E_FAIL;
	
	return S_OK;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);

}

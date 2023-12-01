#include "stdafx.h"
#include "..\Public\Demo.h"

CDemo::CDemo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CDemo::CDemo(const CDemo& rhs)
    :CGameObject(rhs)
{
}

HRESULT CDemo::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

	return S_OK;
}

HRESULT CDemo::Render()
{
	/*if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_Buffer();

	if (m_bWireFrame)
		m_pShaderCom->Set_RenderState(CShader::RENDERSTATE::RS_WIREFRAME);
	else
		m_pShaderCom->Set_RenderState(CShader::RENDERSTATE::RS_SOLID);

	m_pVIBufferCom->Render();*/

    return S_OK;
}

void CDemo::Free()
{
    __super::Free();
}

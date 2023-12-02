#include "stdafx.h"
#include "..\Public\Mark.h"

#include "GameInstance.h"

CMark::CMark(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CMark::CMark(const CMark& rhs)
	:CGameObject(rhs)
{
}

HRESULT CMark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMark::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CMark::Priority_Tick(_float fTimeDelta)
{
}

void CMark::Tick(_float fTimeDelta)
{
}

void CMark::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CMark::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CMark::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos", &m_pGameInstance->Get_Camera_Pos(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMark::Ready_Component()
{
	/* For.Com_Buffer*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_Buffer"), reinterpret_cast<CComponent**>(&m_pBufferCom))))
		return E_FAIL;

	/* For.Com_Shader*/
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXPOSTEX"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

CMark* CMark::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMark* pInstance = new CMark(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMark::Clone(void* pArg)
{
	CMark* pInstance = new CMark(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMark");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMark::Free()
{
	__super::Free();

	Safe_Release(m_pBufferCom);
	Safe_Release(m_pShaderCom);
}

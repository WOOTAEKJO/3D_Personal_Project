#include "stdafx.h"
#include "..\Public\Terrain_Demo.h"
#include "GameInstance.h"

CTerrain_Demo::CTerrain_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CTerrain_Demo::CTerrain_Demo(const CTerrain_Demo& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain_Demo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain_Demo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CTerrain_Demo::Priority_Tick(_float fTimeDelta)
{
}

void CTerrain_Demo::Tick(_float fTimeDelta)
{
	if (m_pVIBufferCom == nullptr)
		return;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_pGameInstance->Update_Mouse(g_hWnd);
		_float3	vMousePos;
		m_pVIBufferCom->Compute_MousePos(&vMousePos, m_pTransformCom->Get_WorldMatrix_Matrix());
		m_pVIBufferCom->Update_Buffer(XMLoadFloat3(&vMousePos), m_fRadius, m_fHeight, m_fSharpness);
	}
}

void CTerrain_Demo::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CTerrain_Demo::Render()
{

	if (m_pVIBufferCom != nullptr) {

		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Bind_Buffer();

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CTerrain_Demo::Create_DynamicBuffer(_uint iVerticesXNum, _uint iVerticesZNum)
{
	if (m_pVIBufferCom != nullptr) {
		MSG_BOX("already exists");
		return E_FAIL;
	}

	CVIBuffer_DTerrain::DTERRAINDESC tDTerrainDesc;

	tDTerrainDesc.iVerticesXNum = iVerticesXNum;
	tDTerrainDesc.iVerticesZNum = iVerticesZNum;

	/* For.Com_VIBuffer*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_DTerrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &tDTerrainDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Demo::Delete_DynamicBuffer()
{
	if (m_pVIBufferCom == nullptr) {
		MSG_BOX("No Buffer");
		return E_FAIL;
	}

	if (FAILED(Delete_Component(TEXT("Com_VIBuffer"))))
		return E_FAIL;

	Safe_Release(m_pVIBufferCom);
	
	return S_OK;
}

HRESULT CTerrain_Demo::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Demo::Ready_Component()
{
	
	/* For.Com_Shader*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VTXTBN"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CTerrain_Demo* CTerrain_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain_Demo* pInstance = new CTerrain_Demo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTerrain_Demo::Clone(void* pArg)
{
	CTerrain_Demo* pInstance = new CTerrain_Demo(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain_Demo::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}

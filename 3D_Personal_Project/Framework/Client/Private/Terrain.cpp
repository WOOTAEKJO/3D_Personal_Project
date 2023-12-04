#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CTerrain::CTerrain(const CTerrain& rhs)
	:CGameObject(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CTerrain::Priority_Tick(_float fTimeDelta)
{
}

void CTerrain::Tick(_float fTimeDelta)
{
	/*if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_pGameInstance->Update_Mouse();
		_float3	vMousePos;
		m_pVIBufferCom->Compute_MousePos(&vMousePos, m_pTransformCom->Get_WorldMatrix_Matrix());
		m_pVIBufferCom->Update_Buffer(XMLoadFloat3(&vMousePos), 10.f, 10.f, 0.f);
	}*/
}

void CTerrain::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CTerrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(SHADER_TBN::TBN_TERRAIN);

	m_pVIBufferCom->Bind_Buffer();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
{
	
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
	->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos", &m_pGameInstance->Get_Camera_Pos(), sizeof(_float4))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CTerrain::Ready_Component()
{
	
	//CVIBuffer_DTerrain::DTERRAINDESC tDTerrainDesc;

	//tDTerrainDesc.iVerticesXNum = 300;
	//tDTerrainDesc.iVerticesZNum = 300;

	///* For.Com_VIBuffer*/
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_DTerrain"),
	//	TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &tDTerrainDesc)))
	//	return E_FAIL;

	///* For.Com_Shader*/
	//if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VTXTBN"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	/* For.Com_VIBuffer*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Shader*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VTXNORTEX"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_DIFFUSE]))))
		return E_FAIL;

	/* For.Com_Mask*/
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain_Mask"),
		TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_MASK]))))
		return E_FAIL;
	

	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	for (_uint i = 0; i < TYPE_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}

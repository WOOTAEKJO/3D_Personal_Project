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
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix_Float4x4());
}

void CTerrain::Tick(_float fTimeDelta)
{
	
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

	m_pNavigationCom->Render();

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
	/*if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;*/
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos", 
		&m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS),sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Ready_Component()
{
	wstring strTerrainTag;
	if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_GAMEPLAY)
		strTerrainTag = BUFFER_TERRAIN_TAG;
	else if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_BOSS1)
		strTerrainTag = BUFFER_TERRAIN2_TAG;

	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CVIBuffer_Terrain>(strTerrainTag, &m_pVIBufferCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(TEX_LANDSCAPE_TAG, &m_pTextureCom))) return E_FAIL;
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom))) return E_FAIL;

	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
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

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
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

	/*for (_uint i = 0; i < TYPE_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}*/

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}

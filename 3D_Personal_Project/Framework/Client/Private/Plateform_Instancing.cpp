#include "stdafx.h"
#include "..\Public\Plateform_Instancing.h"

CPlateform_Instancing::CPlateform_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CPlateform_Instancing::CPlateform_Instancing(const CPlateform_Instancing& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlateform_Instancing::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlateform_Instancing::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		PLATEFORM_INSTANCING_DESC* pPlateform = (PLATEFORM_INSTANCING_DESC*)pArg;

		m_strModelTag = pPlateform->strModelTag;
		m_vecInstanceVertex = pPlateform->vecInstanceVertex;

		if (FAILED(Ready_Component()))
			return E_FAIL;
	}
	
	return S_OK;
}

void CPlateform_Instancing::Priority_Tick(_float fTimeDelta)
{
}

void CPlateform_Instancing::Tick(_float fTimeDelta)
{
	
}

void CPlateform_Instancing::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
	/*if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_SHADOW, this)))
		return;*/
}

HRESULT CPlateform_Instancing::Render()
{
	if (m_pModelCom == nullptr)
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (_uint i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CPlateform_Instancing::Render_Shadow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance->Get_ShadowLight()->
		Get_Matrix(CShadowLight::STATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance->Get_ShadowLight()->
		Get_Matrix(CShadowLight::STATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fLightFar", &m_pGameInstance->Get_ShadowLight()->
		Open_Light_Desc()->fFar, sizeof(_float))))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (_uint i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(3);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CPlateform_Instancing::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlateform_Instancing::Ready_Component()
{

	if (FAILED(Add_Component<CShader>(SHADER_MESHINSTANCING_TAG, &m_pShaderCom))) return E_FAIL;

	CModel_Instancing::MESH_INSTANCE_DESC Desc = {};
	Desc.vecInstanceVertex = m_vecInstanceVertex;
	if (FAILED(Add_Component<CModel_Instancing>(m_strModelTag, &m_pModelCom,&Desc))) return E_FAIL;

	return S_OK;
}

CPlateform_Instancing* CPlateform_Instancing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlateform_Instancing* pInstance = new CPlateform_Instancing(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlateform_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlateform_Instancing::Clone(void* pArg)
{
	CPlateform_Instancing* pInstance = new CPlateform_Instancing(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlateform_Instancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlateform_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

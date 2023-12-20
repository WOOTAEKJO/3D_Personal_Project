#include "stdafx.h"
#include "..\Public\SkyBox.h"

#include "GameInstance.h"

CSkyBox::CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkyBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CSkyBox::Priority_Tick(_float fTimeDelta)
{
	
}

void CSkyBox::Tick(_float fTimeDelta)
{
	
}

void CSkyBox::Late_Tick(_float fTimeDelta)
{

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMLoadFloat4(&m_pGameInstance->Get_Camera_Pos()));

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return;
	
}

HRESULT CSkyBox::Render()
{
	
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}


HRESULT CSkyBox::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 2)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CSkyBox::Ready_Component()
{
	
	/* For.Com_Shader*/ 
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), SHADER_CUBE_TAG,
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Buffer*/
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), BUFFER_CUBE_TAG,
		TEXT("Com_Buffer"), reinterpret_cast<CComponent**>(&m_pBufferCom))))
		return E_FAIL;

	/* For.Com_Texture*/
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), TEX_SKYBOX_TAG,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CSkyBox* CSkyBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkyBox* pInstance = new CSkyBox(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSkyBox::Clone(void* pArg)
{
	CSkyBox* pInstance = new CSkyBox(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkyBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
	
}

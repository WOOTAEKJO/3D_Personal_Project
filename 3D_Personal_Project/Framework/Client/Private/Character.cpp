#include "stdafx.h"
#include "..\Public\Character.h"

#include "GameInstance.h"

CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& rhs)
	:CGameObject(rhs)
{
}

HRESULT CCharacter::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCharacter::Priority_Tick(_float fTimeDelta)
{
	CGameObject::Priority_Tick(fTimeDelta);
}

void CCharacter::Tick(_float fTimeDelta)
{
	CGameObject::Tick(fTimeDelta);
}

void CCharacter::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
	CGameObject::Late_Tick(fTimeDelta);
}

HRESULT CCharacter::Render()
{
	/*if (FAILED(Bind_ShaderResources()))
		return E_FAIL;*/

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(SHADER_TBN::TBN_MODEL);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pNavigationCom->Render();
#endif

	return S_OK;
}

HRESULT CCharacter::Bind_ShaderResources()
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

HRESULT CCharacter::Ready_Component()
{
	/* For.Com_Shader*/
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), SHADER_ANIMMESH_TAG,
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), m_strModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Navigation*/
	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = 0;

	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), COM_NAVIGATION_TAG,
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	return S_OK;
}

void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
}

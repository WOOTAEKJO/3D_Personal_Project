#include "stdafx.h"
#include "..\Public\UI_ChatBox.h"

#include "GameInstance.h"

CUI_ChatBox::CUI_ChatBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CUI_ChatBox::CUI_ChatBox(const CUI_ChatBox& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ChatBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ChatBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CUI_ChatBox::Priority_Tick(_float fTimeDelta)
{
}

void CUI_ChatBox::Tick(_float fTimeDelta)
{
	
}

void CUI_ChatBox::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CUI_ChatBox::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChatBox::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChatBox::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

CUI_ChatBox* CUI_ChatBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ChatBox* pInstance = new CUI_ChatBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ChatBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_ChatBox::Clone(void* pArg)
{
	CUI_ChatBox* pInstance = new CUI_ChatBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_ChatBox::Free()
{
	__super::Free();

}

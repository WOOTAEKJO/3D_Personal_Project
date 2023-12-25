#include "stdafx.h"
#include "..\Public\Monster.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster& rhs)
	:CCharacter(rhs)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	
	if (FAILED(CCharacter::Initialize(&pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Tick(_float fTimeDelta)
{
	CCharacter::Priority_Tick(fTimeDelta);
}

void CMonster::Tick(_float fTimeDelta)
{
	CCharacter::Tick(fTimeDelta);
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	CCharacter::Late_Tick(fTimeDelta);
}

HRESULT CMonster::Render()
{
	if (FAILED(CCharacter::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(CCharacter::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_Component()
{
	if (FAILED(CCharacter::Ready_Component()))
		return E_FAIL;

	

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pAICom);
}

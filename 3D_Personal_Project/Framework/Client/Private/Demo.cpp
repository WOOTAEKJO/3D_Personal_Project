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
	
    return S_OK;
}

void CDemo::Free()
{
    __super::Free();
}

#include "..\Public\Cell.h"
#include "VIBuffer_Cell.h"
#include "Shader.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
#ifdef _DEBUG
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

#endif
}

HRESULT CCell::Initialize(FLOAT3X3 pPoints)
{
	memcpy(&m_pPoints, &pPoints, sizeof(_float3));

#ifdef _DEBUG
	m_pBufferCom = CVIBuffer_Cell::Create(m_pDevice,m_pContext, m_pPoints);
	if (m_pBufferCom == nullptr)
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CCell::Render(CShader* pShader, _float4x4 matView, _float4x4 matProj)
{

	_float4x4		matWorld;
	XMStoreFloat4x4(&matWorld, XMMatrixIdentity());

	if (FAILED(pShader->Bind_Matrix("g_matWorld", &matWorld)))
		return E_FAIL;
	if (FAILED(pShader->Bind_Matrix("g_matView", &matView)))
		return E_FAIL;
	if (FAILED(pShader->Bind_Matrix("g_matProj", &matProj)))
		return E_FAIL;
;
	pShader->Begin(0);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FLOAT3X3 pPoints)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints))) {
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pBufferCom);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#endif
}

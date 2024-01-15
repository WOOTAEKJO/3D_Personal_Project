#include "..\Public\Light.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

CLight::CLight()
{
}

HRESULT CLight::Initialize(const LIGHT_DESC& eLightDesc)
{
	m_eLight_Desc = eLightDesc;

	return S_OK;
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer_Rect* pBuffer)
{
	_uint iPassIndx = 0;

	if (m_eLight_Desc.eType == LIGHT_DESC::LIGHT_TYPE::TYPE_DIRECTION)
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_eLight_Desc.vDirection, sizeof(_float4))))
			return E_FAIL;

		iPassIndx = 1;
	}
	else if (m_eLight_Desc.eType == LIGHT_DESC::LIGHT_TYPE::TYPE_POINT)
	{
		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_eLight_Desc.vPos, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pShader->Bind_RawValue("g_fLightRange", &m_eLight_Desc.fRange, sizeof(_float))))
			return E_FAIL;

		iPassIndx = 2;
	}

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_eLight_Desc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_eLight_Desc.vAmbient, sizeof(_float4))))
		return E_FAIL;

	pShader->Begin(iPassIndx);

	pBuffer->Bind_Buffer();

	return pBuffer->Render();
}

CLight* CLight::Create(const LIGHT_DESC& eLightDesc)
{
	CLight* pInstance = new CLight();

	if (FAILED(pInstance->Initialize(eLightDesc))) {
		MSG_BOX("Failed to Created : CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	__super::Free();
}

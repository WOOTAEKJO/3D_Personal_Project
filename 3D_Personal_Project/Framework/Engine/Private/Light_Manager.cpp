#include "..\Public\Light_Manager.h"

#include "Shader.h"
#include "Light.h"

CLight_Manager::CLight_Manager()
{
}

HRESULT CLight_Manager::Initialize()
{
	return S_OK;
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& eLightDesc)
{
	CLight* pLight = CLight::Create(eLightDesc);
	if (pLight == nullptr)
		return E_FAIL;

	m_listLight.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pBuffer)
{
	for (auto& iter : m_listLight)
	{
		iter->Render(pShader,pBuffer);
	}

	return S_OK;
}

CLight_Manager* CLight_Manager::Create()
{
	CLight_Manager* pInstance = new CLight_Manager();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CLight_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight_Manager::Free()
{
	__super::Free();

	for (auto& iter : m_listLight)
		Safe_Release(iter);
	m_listLight.clear();
}

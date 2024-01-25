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

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& eLightDesc,CLight** ppLight)
{
	CLight* pLight = CLight::Create(eLightDesc);
	if (pLight == nullptr)
		return E_FAIL;

	if (ppLight != nullptr) {
		*ppLight = pLight;
	}

	m_listLight.push_back(pLight);

	return S_OK;
}

void CLight_Manager::Delete_Light(CLight* ppLight)
{
	if (ppLight == nullptr)
		return;

	Safe_Release(ppLight);
	ppLight = nullptr;

	/*for (auto& iter = m_listLight.begin(); iter != m_listLight.end();)
	{
		if (*iter == nullptr)
		{
			iter = m_listLight.erase(iter);
		}
		else {
			iter++;
		}
	}*/
}

HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pBuffer)
{
	for (auto& iter : m_listLight)
	{
		//if(iter != nullptr)
		iter->Render(pShader, pBuffer);
			
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

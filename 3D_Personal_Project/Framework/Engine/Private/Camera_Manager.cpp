#include "..\Public\Camera_Manager.h"

#include "Camera.h"

CCamera_Manager::CCamera_Manager()
{
}

HRESULT CCamera_Manager::Initialize()
{
    return S_OK ;
}

HRESULT CCamera_Manager::Add_Camera(const wstring& strCameraTag, CCamera* pCamera)
{
	if (pCamera == nullptr)
		return E_FAIL;

	CCamera* pCurCamera = Find_Camera(strCameraTag);
	if (pCurCamera != nullptr)
		return E_FAIL;

	m_mapCamera.emplace(strCameraTag, pCamera);

	return S_OK;
}

void CCamera_Manager::SetUp_Offset(_float3 vOffset)
{
	CCamera* pCamera = Find_Camera(m_strCurrentCameraTag);
	if (pCamera == nullptr)
		return;

	/*pCamera->SetUp_Offset(vOffset);*/
}

void CCamera_Manager::SetUp_Camera(const wstring& strCameraTag)
{
	CCamera* pCamera = Find_Camera(strCameraTag);
	if (pCamera == nullptr)
		return;

	
}

CCamera* CCamera_Manager::Find_Camera(const wstring& strCameraTag)
{
	auto& iter = m_mapCamera.find(strCameraTag);
	if(iter == m_mapCamera.end())
		return nullptr;

	return iter->second;
	
}

CCamera_Manager* CCamera_Manager::Create()
{
	CCamera_Manager* pInstance = new CCamera_Manager();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CCamera_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Manager::Free()
{
	__super::Free();
}

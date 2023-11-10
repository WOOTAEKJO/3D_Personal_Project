#include "..\Public\EventManager.h"

CEventManager::CEventManager()
{
}

HRESULT CEventManager::Initialize()
{
	return S_OK;
}

HRESULT CEventManager::Add_Event(const wstring&, function<void()>)
{
	return S_OK;
}

CEventManager* CEventManager::Create()
{
	CEventManager* pInstance = new CEventManager();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CEventManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEventManager::Free()
{
	__super::Free();
}

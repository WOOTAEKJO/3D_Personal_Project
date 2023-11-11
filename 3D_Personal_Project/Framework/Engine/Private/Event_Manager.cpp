#include "..\Public\Event_Manager.h"
#include "Event_Function.h"

CEvent_Manager::CEvent_Manager()
{
}

HRESULT CEvent_Manager::Initialize()
{
	return S_OK;
}

HRESULT CEvent_Manager::Add_Event(const wstring& strEventTag, function<void()> pFunction)
{
	if (pFunction == nullptr)
		return E_FAIL;

	CEvent_Function* pEventFunction = Find_Event(strEventTag);

	if (pEventFunction == nullptr)
	{
		pEventFunction = CEvent_Function::Create();
		if (pEventFunction == nullptr)
			return E_FAIL;

		pEventFunction->Add_Function(pFunction);

		m_mapEvent.emplace(strEventTag, pEventFunction);
	}
	else {
		pEventFunction->Add_Function(pFunction);
	}
		
	return S_OK;
}

HRESULT CEvent_Manager::Execute_Event(const wstring& strEventTag)
{
	CEvent_Function* pEventFunction = Find_Event(strEventTag);

	if (pEventFunction == nullptr)
		return E_FAIL;

	pEventFunction->Execute_Function();

	return S_OK;
}

CEvent_Function* CEvent_Manager::Find_Event(const wstring& strEventTag)
{
	auto& iter = m_mapEvent.find(strEventTag);

	if (iter == m_mapEvent.end())
		return nullptr;

	return iter->second;
}

CEvent_Manager* CEvent_Manager::Create()
{
	CEvent_Manager* pInstance = new CEvent_Manager();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CEvent_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEvent_Manager::Free()
{
	__super::Free();

	for (auto& iter : m_mapEvent)
		Safe_Release(iter.second);
	m_mapEvent.clear();
}

#include "..\Public\Event_Function.h"

CEvent_Function::CEvent_Function()
{
}

HRESULT CEvent_Function::Add_Function(function<void()> pFunction)
{
	m_vecFunction.push_back(pFunction);

	return S_OK;
}

void CEvent_Function::Execute_Function()
{
	for (auto& iter : m_vecFunction) {
		iter();
	}
}

CEvent_Function* CEvent_Function::Create()
{
	return new CEvent_Function();
}

void CEvent_Function::Free()
{
	/*for (auto& iter : m_vecFunction)
		Safe_Release(iter);

	m_vecFunction.clear();*/
}

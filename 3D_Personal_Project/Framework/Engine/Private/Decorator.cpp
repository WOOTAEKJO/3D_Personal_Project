#include "..\Public\Decorator.h"

CDecorator::CDecorator()
{
}

HRESULT CDecorator::Add_Child(CNode* pChild)
{
	if (pChild == nullptr)
		return E_FAIL;

	if (m_pChild == nullptr)
		m_pChild = pChild;
	//Safe_AddRef(m_pChild);

	return S_OK;
}

void CDecorator::Free()
{
	__super::Free();

	Safe_Release(m_pChild);
}

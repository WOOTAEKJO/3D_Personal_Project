#include "..\Public\Composite.h"

CComposite::CComposite()
{
}

HRESULT CComposite::Add_Children(CNode* pChildNode)
{
	if (pChildNode == nullptr)
		return E_FAIL;

	m_vecChildren.push_back(pChildNode);

	return S_OK;
}

void CComposite::Free()
{
	__super::Free();

	for (auto& iter : m_vecChildren)
		Safe_Release(iter);
	m_vecChildren.clear();

}

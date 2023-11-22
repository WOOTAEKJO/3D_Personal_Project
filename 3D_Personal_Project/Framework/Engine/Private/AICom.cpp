#include "..\Public\AICom.h"
#include "AICom.h"

CAICom::CAICom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CAICom::CAICom(const CAICom& rhs)
	:CComponent(rhs)
{
}

HRESULT CAICom::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CAICom::Initialize(void* pArg)
{
	
	AIDESC* pAIDesc = (AIDESC*)pArg;
	if (pAIDesc == nullptr)
		return E_FAIL;

	for (_uint i = 0; i < pAIDesc->iNodeNum; i++)
	{
		if (pAIDesc->ppNodes[i] != nullptr)
		{
			m_mapTree.emplace(pAIDesc->pstrNodeTags[i], pAIDesc->ppNodes[i]);
		}
	}

	return S_OK;
}

void	CAICom::Tick(_float fTimeDelta)
{
	if (m_pCurrentTree != nullptr)
		m_pCurrentTree->Tick();
}

HRESULT CAICom::Set_Tree(const wstring& strTreeTag)
{
	CNode* pTree = Find_Tree(strTreeTag);
	if (pTree == nullptr)
		return E_FAIL;

	m_pCurrentTree = pTree;
	Safe_AddRef(m_pCurrentTree);

	return S_OK;
}

CNode* CAICom::Find_Tree(const wstring& strTreeTag)
{
	auto& iter = m_mapTree.find(strTreeTag);

	if (iter == m_mapTree.end())
		return nullptr;

	return iter->second;
}

CAICom* CAICom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAICom* pInstance = new CAICom(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CAICom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAICom::Clone(void* pArg)
{
	CAICom* pInstance = new CAICom(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CAICom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAICom::Free()
{
	__super::Free();

	Safe_Release(m_pCurrentTree);

	for (auto& iter : m_mapTree)
		Safe_Release(iter.second);
	m_mapTree.clear();

}

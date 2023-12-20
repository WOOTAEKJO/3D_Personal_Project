#include "..\Public\Component_Manager.h"
#include "Component.h"

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Initialize(const _uint& iLevelNum)
{
	m_iLevelNum = iLevelNum;

	m_mapCom_ProtoType = new PROTOTYPE[iLevelNum];

	return S_OK;
}

HRESULT CComponent_Manager::Add_Component_ProtoType(const _uint& iLevelIndex, const wstring& strProtoTypeTag, CComponent* pComponent)
{
	if (iLevelIndex >= m_iLevelNum)
		return E_FAIL;

	if (pComponent == nullptr)
		return E_FAIL;

	CComponent* pCComponent = Find_Com_ProtoType(iLevelIndex,strProtoTypeTag);
	if (pCComponent != nullptr)
		return E_FAIL;

	m_mapCom_ProtoType[iLevelIndex].emplace(strProtoTypeTag, pComponent);

	return S_OK;
}

CComponent* CComponent_Manager::Add_Component_Clone(const _uint& iLevelIndex, const wstring& strProtoTypeTag, void* pArg)
{
	CComponent* pComponent = Find_Com_ProtoType(iLevelIndex,strProtoTypeTag);
	if (pComponent == nullptr)
	{
		pComponent = Find_Com_ProtoType(0, strProtoTypeTag);
		if (pComponent == nullptr)
			return nullptr;
	}
		

	CComponent* pClone = pComponent->Clone(pArg);
	if (pClone == nullptr)
		return nullptr;
	
	return pClone;
}

void CComponent_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iLevelNum)
		return;

	for (auto& iter : m_mapCom_ProtoType[iLevelIndex])
		Safe_Release(iter.second);
	m_mapCom_ProtoType[iLevelIndex].clear();
}

CComponent_Manager::PROTOTYPE CComponent_Manager::Get_Com_ProtoType(const _uint& iLevelIndex)
{
	return m_mapCom_ProtoType[iLevelIndex];
}

CComponent* CComponent_Manager::Find_Com_ProtoType(const _uint& iLevelIndex, const wstring& strProtoTypeTag)
{
	auto& iter = m_mapCom_ProtoType[iLevelIndex].find(strProtoTypeTag);

	if (iter == m_mapCom_ProtoType[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CComponent_Manager* CComponent_Manager::Create(const _uint& iLevelNum)
{
	CComponent_Manager* pInstance = new CComponent_Manager();

	if (FAILED(pInstance->Initialize(iLevelNum))) {
		MSG_BOX("Failed to Created : CComponent_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CComponent_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iLevelNum; i++)
	{
		for (auto& iter : m_mapCom_ProtoType[i])
			Safe_Release(iter.second);
		m_mapCom_ProtoType[i].clear();
	}

	Safe_Delete_Array(m_mapCom_ProtoType);
}

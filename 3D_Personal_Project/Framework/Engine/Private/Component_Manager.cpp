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
		return nullptr;

	CComponent* pClone = pComponent->Clone(pArg);
	if (pClone == nullptr)
		return nullptr;

	m_vecClone.push_back(pClone);
	
	return pClone;
}

void CComponent_Manager::Priority_Tick(_float fTimeDelta)
{
	for (auto& iter : m_vecClone)
		iter->Priority_Tick(fTimeDelta);
		
}

void CComponent_Manager::Tick(_float fTimeDelta)
{
	for (auto& iter : m_vecClone)
		iter->Tick(fTimeDelta);
}

void CComponent_Manager::Late_Tick(_float fTimeDelta)
{
	for (auto& iter : m_vecClone)
		iter->Late_Tick(fTimeDelta);
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

	for (auto& iter : m_vecClone)
		Safe_Release(iter);
	m_vecClone.clear();

}

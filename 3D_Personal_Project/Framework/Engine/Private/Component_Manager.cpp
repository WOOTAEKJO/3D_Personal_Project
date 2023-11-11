#include "..\Public\Component_Manager.h"
#include "Component.h"

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Initialize()
{
	return S_OK;
}

HRESULT CComponent_Manager::Add_Component_ProtoType(const wstring& strProtoTypeTag, CComponent* pComponent)
{
	if (pComponent == nullptr)
		return E_FAIL;

	CComponent* pCComponent = Find_Com_ProtoType(strProtoTypeTag);
	if (pCComponent != nullptr)
		return E_FAIL;

	m_mapCom_ProtoType.emplace(strProtoTypeTag, pComponent);

	return S_OK;
}

CComponent* CComponent_Manager::Add_Component_Clone(const wstring& strProtoTypeTag, void* pArg)
{
	CComponent* pComponent = Find_Com_ProtoType(strProtoTypeTag);
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

CComponent* CComponent_Manager::Find_Com_ProtoType(const wstring& strProtoTypeTag)
{
	auto& iter = m_mapCom_ProtoType.find(strProtoTypeTag);

	if (iter == m_mapCom_ProtoType.end())
		return nullptr;

	return iter->second;
}

CComponent_Manager* CComponent_Manager::Create()
{
	CComponent_Manager* pInstance = new CComponent_Manager();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CComponent_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CComponent_Manager::Free()
{
	__super::Free();

	for (auto& iter : m_mapCom_ProtoType)
		Safe_Release(iter.second);
	m_mapCom_ProtoType.clear();
}

#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Initialize(_uint iNumLevel)
{
	m_iNumLevel = iNumLevel;

	m_mapLayer = new LAYERS[iNumLevel];

	return S_OK;
}

HRESULT CObject_Manager::Add_ProtoType(const wstring& strProtoTypeTag, CGameObject* pGameObeject)
{
	if (pGameObeject == nullptr)
		return E_FAIL;

	CGameObject* pGameObject = Find_ProtoType(strProtoTypeTag);

	if (pGameObject != nullptr)
		return E_FAIL;

	m_mapProtoType.emplace(strProtoTypeTag, pGameObeject);

	return S_OK;
}

HRESULT CObject_Manager::Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strProtoTypeTag, CGameObject** ppOut, void* pArg)
{
	CGameObject* pProtoType = Find_ProtoType(strProtoTypeTag);
	if (pProtoType == nullptr)
		return E_FAIL;

	CGameObject* pClone = pProtoType->Clone(pArg);
	if (pClone == nullptr)
		return E_FAIL;

	pClone->Set_ProtoTag(strProtoTypeTag);
	pClone->Set_LayerName(strLayerTag);

	if (iLevelIndex >= m_iNumLevel)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (pLayer == nullptr) {

		pLayer = CLayer::Create();
		if (pLayer == nullptr)
			return E_FAIL;
		
		pLayer->Add_GameObject(pClone);

		m_mapLayer[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else {
		pLayer->Add_GameObject(pClone);
	}

	if (ppOut != nullptr) {
		*ppOut = pClone;
	}
	
	return S_OK;
}

CGameObject* CObject_Manager::Add_Independent_Clone(const wstring& strProtoTypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_ProtoType(strProtoTypeTag);

	if (pPrototype == nullptr)
		return nullptr;

	CGameObject* pClone = pPrototype->Clone(pArg);

	return pClone;
}

void CObject_Manager::Priority_Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevel; i++) {
		for (auto& pLayer : m_mapLayer[i]) {
			if (pLayer.second != nullptr) {
				pLayer.second->Priority_Tick(fTimeDelta);
			}
		}
	}
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevel; i++) {
		for (auto& pLayer : m_mapLayer[i]) {
			if (pLayer.second != nullptr) {
				pLayer.second->Tick(fTimeDelta);
			}
		}
	}
}

void CObject_Manager::Late_Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevel; i++) {
		for (auto& pLayer : m_mapLayer[i]) {
			if (pLayer.second != nullptr) {
				pLayer.second->Late_Tick(fTimeDelta);
			}
		}
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevel)
		return;

	for (auto& pLayer : m_mapLayer[iLevelIndex])
		Safe_Release(pLayer.second);

	m_mapLayer[iLevelIndex].clear();
}

void CObject_Manager::Delete_Objects()
{
	for (size_t i = 0; i < m_iNumLevel; i++) {
		for (auto& iter : m_mapLayer[i])
			iter.second->Delete_Object();
	}
}

list<class CGameObject*> CObject_Manager::Get_ObjectList(_uint iLevelIndex, const wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (pLayer == nullptr)
		list<class CGameObject*>();

	return pLayer->Get_ObjectList();
}

CGameObject* CObject_Manager::Find_ProtoType(const wstring& strProtoTypeTag)
{
	auto& iter = m_mapProtoType.find(strProtoTypeTag);

	if (iter == m_mapProtoType.end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (iLevelIndex >= m_iNumLevel)
		return nullptr;

	auto& iter = m_mapLayer[iLevelIndex].find(strLayerTag);

	if (iter == m_mapLayer[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CObject_Manager* CObject_Manager::Create(_uint iNumLevel)
{
	CObject_Manager* pInstance = new CObject_Manager;

	if (FAILED(pInstance->Initialize(iNumLevel))) {
		MSG_BOX("Failed to Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{
	for (auto& pGameObject : m_mapProtoType) {
		Safe_Release(pGameObject.second);
	}
	m_mapProtoType.clear();

	for (size_t i = 0; i < m_iNumLevel; i++) {
		for (auto& pLayer : m_mapLayer[i]) {

			Safe_Release(pLayer.second);
		}
		m_mapLayer[i].clear();
	}
	Safe_Delete_Array(m_mapLayer);
}

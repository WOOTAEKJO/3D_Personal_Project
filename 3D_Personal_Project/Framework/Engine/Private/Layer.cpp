#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(class CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return E_FAIL;

	m_listClone.push_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_listClone) {

		if (pGameObject != nullptr)
			pGameObject->Priority_Tick(fTimeDelta);
	}
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_listClone) {

		if (pGameObject != nullptr)
			pGameObject->Tick(fTimeDelta);
	}
}

void CLayer::Late_Tick(_float fTimeDelta)
{
	/*for (auto& pGameObject : m_listClone) {

		if (pGameObject != nullptr)
		{
			pGameObject->Late_Tick(fTimeDelta);

			if (pGameObject->Get_Dead())
			{
				Safe_Release(pGameObject);
				m_listClone.erase(pGameObject);
			}
		}
	}*/

	for (auto& iter = m_listClone.begin(); iter != m_listClone.end();)
	{
		if ((*iter) != nullptr)
		{
			(*iter)->Late_Tick(fTimeDelta);

			if ((*iter)->Get_Dead())
			{
				Safe_Release((*iter));
				iter = m_listClone.erase(iter);
			}
			else {
				iter++;
			}
		}
		else {
			iter++;
		}
	}
}

CLayer* CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_listClone) {
		Safe_Release(pGameObject);
	}

	m_listClone.clear();
}

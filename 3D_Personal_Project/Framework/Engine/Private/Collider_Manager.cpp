#include "..\Public\Collider_Manager.h"
#include "GameInstance.h"

#include "Collider_Layer.h"
#include "GameObject.h"

CCollider_Manager::CCollider_Manager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CCollider_Manager::Initialize()
{
	return S_OK;
}

void CCollider_Manager::Update()
{
	for (auto& Layer : m_vecActiveCollider)
	{
		for (auto& SourColl : Layer.first->Get_ColliderList())
		{
			for (auto& DestColl : Layer.second->Get_ColliderList())
			{
				if (SourColl == DestColl)
					continue;

				Check_Collision(SourColl, DestColl);
			}
		}
	}

	//Delete_Collider();
}

HRESULT CCollider_Manager::Add_Collision(_uint iColLayer, CCollider* pCollider)
{
	if (pCollider == nullptr)
		return E_FAIL;

	CCollider_Layer* pLayer = Find_Collider_Layer(iColLayer);
	if (pLayer == nullptr)
	{
		pLayer = CCollider_Layer::Create();
		if (pLayer == nullptr)
			return E_FAIL;

		if (FAILED(pLayer->Add_Collider(pCollider)))
			return E_FAIL;
		pCollider->Set_ColLayer_Type(iColLayer);

		m_mapColliderLayer.emplace(iColLayer, pLayer);

	}
	else {
		if (FAILED(pLayer->Add_Collider(pCollider)))
			return E_FAIL;
		pCollider->Set_ColLayer_Type(iColLayer);
	}

	return S_OK;
}

HRESULT CCollider_Manager::Add_Pair_Collision(_uint iSourColLayer, _uint iDestColLayer)
{
	CCollider_Layer* pSourLayer = Find_Collider_Layer(iSourColLayer);
	CCollider_Layer* pDestLayer = Find_Collider_Layer(iDestColLayer);

	if (pSourLayer == nullptr || pDestLayer == nullptr)
		return E_FAIL;

	m_vecActiveCollider.push_back(make_pair(pSourLayer, pDestLayer));

	return S_OK;
}

void CCollider_Manager::Check_Collision(CCollider* SourpCollider, CCollider* DestpCollider)
{
	
	COLLIDER_ID ID;
	ID.iSour_ID = SourpCollider->Get_Collider_ID();
	ID.iDest_ID = DestpCollider->Get_Collider_ID();

	auto iter = m_mapJudgeColl.find(ID.ID);

	if (iter == m_mapJudgeColl.end())
	{
		m_mapJudgeColl.emplace(ID.ID, false);
		iter = m_mapJudgeColl.find(ID.ID);
	}

	/*if (m_mapJudgeColl.empty())
		return;*/

	if (!SourpCollider->Get_UseCol() || !DestpCollider->Get_UseCol())
	{
		SourpCollider->OnCollisionExit(DestpCollider);
		DestpCollider->OnCollisionExit(SourpCollider);
		iter->second = false;
		return;
	}

	if (SourpCollider->Collision(DestpCollider))
	{
		if (iter->second) // 총돌을 했었는지
		{
			if (SourpCollider->Get_Owner()->Get_Dead() || DestpCollider->Get_Owner()->Get_Dead())
			{
				SourpCollider->OnCollisionExit(DestpCollider);
				DestpCollider->OnCollisionExit(SourpCollider);
				iter->second = false;
			}
			else {
				SourpCollider->OnCollisionStay(DestpCollider);
				DestpCollider->OnCollisionStay(SourpCollider);
			}
		}
		else {							// 충돌이 처음이면
			if (SourpCollider->Get_Owner()->Get_Dead() || DestpCollider->Get_Owner()->Get_Dead())
			{
				SourpCollider->OnCollisionExit(DestpCollider);
				DestpCollider->OnCollisionExit(SourpCollider);
				iter->second = false;
			}
			else {
				SourpCollider->OnCollisionEnter(DestpCollider);
				DestpCollider->OnCollisionEnter(SourpCollider);
				if (m_mapJudgeColl.empty())
					return;

				iter->second = true;
			}
		}
	}
	else {
		if (iter->second) // 총돌을 했었는데, 충돌이 끝났다면
		{
			SourpCollider->OnCollisionExit(DestpCollider);
			DestpCollider->OnCollisionExit(SourpCollider);
			iter->second = false;
		}
	}
}

CCollider_Layer* CCollider_Manager::Find_Collider_Layer(_uint iColLayer)
{
	auto& iter = m_mapColliderLayer.find(iColLayer);
	if (iter == m_mapColliderLayer.end())
		return nullptr;

	return iter->second;
}

void CCollider_Manager::Delete_Collider()
{
	
	for (auto& iter : m_vecActiveCollider)
	{
		iter.first->Delete_Collider();
		iter.second->Delete_Collider();
	}
}

void CCollider_Manager::Clear()
{
	for (auto& iter : m_mapColliderLayer)
	{
		Safe_Release(iter.second);
	}
	m_mapColliderLayer.clear();
	m_vecActiveCollider.clear();
	m_mapJudgeColl.clear();
}

CCollider_Manager* CCollider_Manager::Create()
{
	CCollider_Manager* pInstance = new CCollider_Manager();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CCollider_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

	for (auto& iter : m_mapColliderLayer)
	{
		Safe_Release(iter.second);
	}
	m_mapColliderLayer.clear();
}

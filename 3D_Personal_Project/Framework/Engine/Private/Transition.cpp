#include "..\Public\Transition.h"

CTransition::CTransition()
{
}

HRESULT CTransition::Initialize()
{
    return S_OK;
}

HRESULT CTransition::Add_Transition(function<bool()> pFunction, const _uint& iResultStateID)
{
	if (pFunction == nullptr)
		return E_FAIL;

	function<bool()> pFFunction = Find_Transition(iResultStateID);
	if (pFFunction != nullptr)
		return E_FAIL;

	m_mapTransition.emplace(iResultStateID, pFunction);

	return S_OK;
}

bool CTransition::Is_Transition(_uint* iResultStateID)
{
	for (auto& iter : m_mapTransition){
		if (iter.second()) {
			*iResultStateID = iter.first;
			return true;
		}
	}
	return false;
}

function<bool()> CTransition::Find_Transition(const _uint& iResultStateID)
{
	auto& iter = m_mapTransition.find(iResultStateID);

	if (iter == m_mapTransition.end())
		return nullptr;

	return iter->second;
}

CTransition* CTransition::Create()
{
	CTransition* pInstance = new CTransition();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CTransition");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransition::Free()
{
    __super::Free();
}

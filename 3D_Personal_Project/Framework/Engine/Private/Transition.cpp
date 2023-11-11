#include "..\Public\Transition.h"

CTransition::CTransition()
{
}

HRESULT CTransition::Initialize()
{
    return S_OK;
}

HRESULT CTransition::Add_Transition(function<bool()> pFunction, const wstring& strResultStateTag)
{
	function<bool()> pFFunction = Find_Transition(strResultStateTag);
	if (pFFunction != nullptr)
		return E_FAIL;

	m_mapTransition.emplace(strResultStateTag, pFunction);

	return S_OK;
}

bool CTransition::Is_Transition(wstring* strResultStateTag)
{
	for (auto& iter : m_mapTransition){
		if (iter.second()) {
			*strResultStateTag = iter.first;
			return true;
		}
	}
	return false;
}

function<bool()> CTransition::Find_Transition(const wstring& strResultStateTag)
{
	auto& iter = m_mapTransition.find(strResultStateTag);

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

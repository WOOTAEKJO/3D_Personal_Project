#include "..\Public\Selector.h"

CSelector::CSelector()
{
}

HRESULT CSelector::Initialize()
{
	m_iter = m_vecChildren.begin();

	return S_OK;
}

CNode::STATUS CSelector::Update()
{
	if (m_vecChildren.empty())
		return STATUS::FAILURE;

	while (m_iter != m_vecChildren.end())
	{
		auto Status = (*m_iter)->Tick();

		if (Status != STATUS::FAILURE)
			return Status;

		m_iter++;
	}

	return STATUS::FAILURE;
}

HRESULT CSelector::Terminate(STATUS eStatus)
{
	return S_OK;
}

CSelector* CSelector::Create()
{
	CSelector* pInstance = new CSelector();

	/*if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CBlackBoard");
		Safe_Release(pInstance);
	}*/

	return pInstance;
}

void CSelector::Free()
{
	__super::Free();
}

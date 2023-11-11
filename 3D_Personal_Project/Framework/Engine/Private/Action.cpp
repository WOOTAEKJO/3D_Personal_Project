#include "..\Public\Action.h"

CAction::CAction()
{
}

HRESULT CAction::Initialize()
{
    return S_OK;
}

HRESULT CAction::Add_Action(function<void()> pFunction)
{
	if (pFunction == nullptr)
		return E_FAIL;

	m_vecAction.push_back(pFunction);

	return S_OK;
}

void CAction::Tick(_float fTimeDelta)
{
	for (auto& iter : m_vecAction)
		iter();
}

CAction* CAction::Create()
{
	CAction* pInstance = new CAction();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CAction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAction::Free()
{
	__super::Free();

	
}

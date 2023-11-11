#include "..\Public\Action.h"

CAction::CAction()
{
}

HRESULT CAction::Initialize()
{
    return S_OK;
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

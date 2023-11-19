#include "..\Public\BlackBoard.h"

CBlackBoard::CBlackBoard()
{
}

HRESULT CBlackBoard::Initialize()
{
	return S_OK;
}

CBlackBoard* CBlackBoard::Create()
{
	CBlackBoard* pInstance = new CBlackBoard();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CBlackBoard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackBoard::Free()
{
	__super::Free();
}

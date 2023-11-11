#include "..\Public\State.h"

CState::CState()
{
}

HRESULT CState::Initialize()
{
    return S_OK;
}

void CState::Free()
{
	__super::Free();
}

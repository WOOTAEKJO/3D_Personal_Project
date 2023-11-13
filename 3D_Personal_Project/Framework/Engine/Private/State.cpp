#include "..\Public\State.h"
#include "Action.h"
#include "Transition.h"

CState::CState()
{
}

HRESULT CState::Initialize(CGameObject* pGameObject)
{
	
    return S_OK;
}


void CState::Free()
{
	__super::Free();
}

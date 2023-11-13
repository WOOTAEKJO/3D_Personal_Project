#include "..\Public\State.h"
#include "Action.h"
#include "Transition.h"

CState::CState(CStateMachine* pStateMachine)
	:m_pStateMachine(pStateMachine)
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

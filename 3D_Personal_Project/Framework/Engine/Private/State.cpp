#include "..\Public\State.h"
#include "Action.h"
#include "Transition.h"

#include "GameInstance.h"

CState::CState()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CState::Initialize(CGameObject* pGameObject)
{
	
    return S_OK;
}


void CState::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}

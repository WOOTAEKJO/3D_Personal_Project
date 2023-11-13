#include "stdafx.h"
#include "..\Public\StateCharge.h"
#include "StateMachine.h"
#include "GameObject.h"

CStateCharge::CStateCharge(CGameObject* pOwner, CStateMachine* pStateManchine)
	:m_pOwner(pOwner),
	m_pStateMachine(pStateManchine)
{
	//Safe_AddRef(m_pOwner);
	//Safe_AddRef(m_pStateMachine);
}

HRESULT CStateCharge::Initialize()
{
	return S_OK;
}

void CStateCharge::Free()
{
	__super::Free();

	//Safe_Release(m_pOwner);
	//Safe_Release(m_pStateMachine);
}

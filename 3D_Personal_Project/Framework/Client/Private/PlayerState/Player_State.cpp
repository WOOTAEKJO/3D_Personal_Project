#include "stdafx.h"
#include "..\Public\Player_State.h"
#include "StateMachine.h"


CPlayer_State::CPlayer_State()
{
}

HRESULT CPlayer_State::Initialize(CGameObject* pGameObject)
{
	m_pOwner = dynamic_cast<CPlayer*>(pGameObject);
	if (m_pOwner == nullptr)
		return E_FAIL;

	return S_OK;
}

void CPlayer_State::State_Enter()
{
}

_uint CPlayer_State::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPlayer_State::State_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPlayer_State::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPlayer_State::State_Exit()
{
}

void CPlayer_State::Free()
{
	__super::Free();
}

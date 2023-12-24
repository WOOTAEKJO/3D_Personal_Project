#include "stdafx.h"
#include "..\Public\PlayerState\Player_State.h"
#include "StateMachine.h"


CPlayer_State::CPlayer_State()
{
}

HRESULT CPlayer_State::Initialize(CGameObject* pGameObject)
{
	m_pOwner = dynamic_cast<CPlayer*>(pGameObject);
	if (m_pOwner == nullptr)
		return E_FAIL;

	m_pOwnerModel = m_pOwner->Get_BodyModel();
	Safe_AddRef(m_pOwnerModel);
	m_pOnwerTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pOnwerTransform);
	m_pOnwerNavigation = m_pOwner->Get_Component<CNavigation>();
	Safe_AddRef(m_pOnwerNavigation);

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

void CPlayer_State::Translate(CTransform::STATE eType,_float fSpeed, _float fTimeDelta, _bool bTurn)
{
	_vector vDir = m_pOnwerTransform->Get_State(eType);
	_vector vPos = XMVector3Normalize(vDir) * fSpeed * fTimeDelta
		* (bTurn == false ? 1.f : -1.f);
	m_pOnwerTransform->Translate(vPos, m_pOnwerNavigation);
}

void CPlayer_State::Free()
{
	__super::Free();

	Safe_Release(m_pOwnerModel);
	Safe_Release(m_pOnwerTransform);
	Safe_Release(m_pOnwerNavigation);
}

#include "stdafx.h"
#include "..\Public\PlayerState\Player_State.h"
#include "StateMachine.h"

#include "Player_Weapon_Spear.h"

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
	m_pOnwerRigidBody = m_pOwner->Get_Component<CRigidBody>();
	Safe_AddRef(m_pOnwerRigidBody);
	m_pOnwerController = m_pOwner->Get_Component<CController>();
	Safe_AddRef(m_pOnwerController);

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

void CPlayer_State::Trans_Attack(_bool bCheck)
{
	m_pOwner->Get_WeaponCollider()->Set_UseCol(bCheck);
}

void CPlayer_State::Free()
{
	__super::Free();

}

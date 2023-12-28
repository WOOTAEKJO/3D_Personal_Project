#include "stdafx.h"
#include "..\Public\MonsterState\Monster_State.h"
#include "StateMachine.h"


CMonster_State::CMonster_State()
{
}

HRESULT CMonster_State::Initialize(CGameObject* pGameObject)
{
	m_pOwner = dynamic_cast<CMonster*>(pGameObject);
	if (m_pOwner == nullptr)
		return E_FAIL;

	m_pOwnerModel = m_pOwner->Get_Component<CModel>();
	Safe_AddRef(m_pOwnerModel);
	m_pOnwerTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pOnwerTransform);
	m_pOnwerNavigation = m_pOwner->Get_Component<CNavigation>();
	Safe_AddRef(m_pOnwerNavigation);

	return S_OK;
}

void CMonster_State::State_Enter()
{
}

_uint CMonster_State::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CMonster_State::State_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CMonster_State::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CMonster_State::State_Exit()
{
}

CMonster::STATE CMonster_State::Dead()
{
	if (m_pOwner->Open_Status_Desc()->iCurHP <= 0)
		return CMonster::STATE::DEAD;
}

void CMonster_State::Free()
{
	__super::Free();
}

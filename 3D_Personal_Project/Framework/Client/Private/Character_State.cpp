#include "stdafx.h"
#include "..\Public\Character_State.h"
#include "StateMachine.h"

CCharacter_State::CCharacter_State()
{
}

HRESULT CCharacter_State::Initialize(CGameObject* pGameObject)
{

	return S_OK;
}

void CCharacter_State::State_Enter()
{
}

_uint CCharacter_State::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CCharacter_State::State_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CCharacter_State::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CCharacter_State::State_Exit()
{
}

void CCharacter_State::Translate(CTransform::STATE eType,_float fSpeed, _float fTimeDelta, _bool bTurn)
{
	_vector vDir = m_pOnwerTransform->Get_State(eType);
	_vector vPos = XMVector3Normalize(vDir) * fSpeed * fTimeDelta
		* (bTurn == false ? 1.f : -1.f);
	m_pOnwerTransform->Translate(vPos, m_pOnwerNavigation, fTimeDelta);
}

void CCharacter_State::Is_Attack_Time(CCollider* pOwnerCol ,_float fTimeDelta, _float fTime)
{
	if (m_bAttack)
		m_fTime += fTimeDelta;

	if (m_fTime > fTime && m_bAttack)
	{
		pOwnerCol->Set_UseCol(true);
		m_bAttack = false;
		m_fTime = 0.f;
	}
}

void CCharacter_State::Reset_Attack_Time(CCollider* pOwnerCol)
{
	pOwnerCol->Set_UseCol(false);
	m_bAttack = true;
}

void CCharacter_State::Free()
{
	__super::Free();

	Safe_Release(m_pOwnerModel);
	Safe_Release(m_pOnwerTransform);
	Safe_Release(m_pOnwerNavigation);
	Safe_Release(m_pOnwerRigidBody);
	Safe_Release(m_pOnwerController);
}

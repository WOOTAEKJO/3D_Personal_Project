#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Exhausted_IDLE.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Exhausted_IDLE::CHelico_Exhausted_IDLE()
{
}

HRESULT CHelico_Exhausted_IDLE::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Exhausted_IDLE::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::EXHAUSTED_IDLE);

	m_pOwner->Open_Status_Desc()->bAttack_able = true;
	m_pOwnerCollider->Set_UseCol(true);
}

_uint CHelico_Exhausted_IDLE::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Exhausted_IDLE::State_Tick(_float fTimeDelta)
{
	if (m_pOwner->Open_Status_Desc()->bHited)
		return CHelicoScarrow::STATE::EXHAUSTED_HIT;

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_Exhausted_IDLE::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CHelico_Exhausted_IDLE::State_Exit()
{
	m_pOwner->Open_Status_Desc()->bAttack_able = false;
	m_pOwnerCollider->Set_UseCol(false);
}

CHelico_Exhausted_IDLE* CHelico_Exhausted_IDLE::Create(CGameObject* pGameObject)
{
	CHelico_Exhausted_IDLE* pInstance = new CHelico_Exhausted_IDLE();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Exhausted_IDLE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Exhausted_IDLE::Free()
{
	__super::Free();

}

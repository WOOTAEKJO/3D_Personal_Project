#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Attack.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Attack::CHelico_Attack()
{
}

HRESULT CHelico_Attack::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Attack::State_Enter()
{
	
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::HELICO_ATTACK);
	
}

_uint CHelico_Attack::State_Priority_Tick(_float fTimeDelta)
{
	m_pOwnerModel;
	return m_iStateID;
}

_uint CHelico_Attack::State_Tick(_float fTimeDelta)
{

	if (!m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(100.f))
	{
		if (m_pOwnerCollider->Get_Collision())
		{
			return CHelicoScarrow::STATE::HELICO_END;
		}

		m_pOwner->Open_Status_Desc()->bAttack_able = true;
		m_pOwnerCollider->Set_UseCol(true);
	}
	else {
		m_pOwner->Open_Status_Desc()->bAttack_able = false;
		m_pOwnerCollider->Set_UseCol(false);
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_Attack::State_Late_Tick(_float fTimeDelta)
{
	m_pOwnerModel;
	return m_iStateID;
}

void CHelico_Attack::State_Exit()
{
	m_pOwner->Open_Status_Desc()->bAttack_able = false;
	m_pOwnerCollider->Set_UseCol(false);
}

CHelico_Attack* CHelico_Attack::Create(CGameObject* pGameObject)
{
	CHelico_Attack* pInstance = new CHelico_Attack();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Attack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Attack::Free()
{
	__super::Free();

}

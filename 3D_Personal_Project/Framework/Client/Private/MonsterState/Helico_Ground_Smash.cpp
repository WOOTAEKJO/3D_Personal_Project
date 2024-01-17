#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Ground_Smash.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Ground_Smash::CHelico_Ground_Smash()
{
}

HRESULT CHelico_Ground_Smash::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Ground_Smash::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::GROUND_SMASH);
}

_uint CHelico_Ground_Smash::State_Priority_Tick(_float fTimeDelta)
{
	if (m_iCount >= 4)
	{
		m_iCount = 0;
		return  CHelicoScarrow::STATE::DIVE;
	}
		

	return m_iStateID;
}

_uint CHelico_Ground_Smash::State_Tick(_float fTimeDelta)
{
	m_pOwner->Turn(fTimeDelta);

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CHelico_Ground_Smash::State_Late_Tick(_float fTimeDelta)
{
	_float fDuration = m_pOwnerModel->CurAnim_Get_Duration(CHelicoScarrow::STATE::GROUND_SMASH);
	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CHelicoScarrow::STATE::GROUND_SMASH, fDuration - 10.f))
	{
		if (m_bAttack)
		{
			dynamic_cast<CHelicoScarrow*>(m_pOwner)->Create_Shock_Wave();
			m_bAttack = false;
		}
		
	}

	if (m_pOwnerModel->Is_Animation_Finished())
	{
		m_iCount += 1;
		m_bAttack = true;
		m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::GROUND_SMASH);
	}

	return m_iStateID;
}

void CHelico_Ground_Smash::State_Exit()
{
	
}

CHelico_Ground_Smash* CHelico_Ground_Smash::Create(CGameObject* pGameObject)
{
	CHelico_Ground_Smash* pInstance = new CHelico_Ground_Smash();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Ground_Smash");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Ground_Smash::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Exhausted_Hit.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Exhausted_Hit::CHelico_Exhausted_Hit()
{
}

HRESULT CHelico_Exhausted_Hit::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Exhausted_Hit::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::EXHAUSTED_HIT);
}

_uint CHelico_Exhausted_Hit::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Exhausted_Hit::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CHelico_Exhausted_Hit::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		
		if (dynamic_cast<CHelicoScarrow*>(m_pOwner)->Is_AllHited(1))
		{
			if (dynamic_cast<CHelicoScarrow*>(m_pOwner)->Is_AllHited(0))
			{
				return CHelicoScarrow::STATE::DEAD;
			}
			else {
				dynamic_cast<CHelicoScarrow*>(m_pOwner)->Reset_Hited();
				return CHelicoScarrow::STATE::EXHAUSTED_WAKE;
			}
		}
		else
		{
			return CHelicoScarrow::STATE::EXHAUSTED_IDLE;
		}
			
	}
	return m_iStateID;
}

void CHelico_Exhausted_Hit::State_Exit()
{
	m_pOwner->Open_Status_Desc()->bHited = false;
}

CHelico_Exhausted_Hit* CHelico_Exhausted_Hit::Create(CGameObject* pGameObject)
{
	CHelico_Exhausted_Hit* pInstance = new CHelico_Exhausted_Hit();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Exhausted_Hit");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Exhausted_Hit::Free()
{
	__super::Free();

}

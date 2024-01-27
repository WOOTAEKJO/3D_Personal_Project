#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Dive_IDLE.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Dive_IDLE::CHelico_Dive_IDLE()
{
}

HRESULT CHelico_Dive_IDLE::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Dive_IDLE::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::DIVE_IDLE);
	m_pOwner->Set_Light_Activate(false);

}

_uint CHelico_Dive_IDLE::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Dive_IDLE::State_Tick(_float fTimeDelta)
{
	

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_Dive_IDLE::State_Late_Tick(_float fTimeDelta)
{
	if (dynamic_cast<CHelicoScarrow*>(m_pOwner)->Is_SubMonster_AllDead())
		return CHelicoScarrow::STATE::DIVE_UP;

	return m_iStateID;
}

void CHelico_Dive_IDLE::State_Exit()
{
	m_pOwner->Set_Light_Activate(true);
}

CHelico_Dive_IDLE* CHelico_Dive_IDLE::Create(CGameObject* pGameObject)
{
	CHelico_Dive_IDLE* pInstance = new CHelico_Dive_IDLE();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Dive_IDLE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Dive_IDLE::Free()
{
	__super::Free();

}

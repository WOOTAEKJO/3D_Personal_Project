#include "stdafx.h"
#include "..\Public\MonsterState\Helico_IDLE.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_IDLE::CHelico_IDLE()
{
}

HRESULT CHelico_IDLE::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_IDLE::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

	if (!m_pOwner->Open_Status_Desc()->bTalk)
		dynamic_cast<CHelicoScarrow*>(m_pOwner)->Dead_CountDown();
}

_uint CHelico_IDLE::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_IDLE::State_Tick(_float fTimeDelta)
{
	if (!m_pOwner->Open_Status_Desc()->bTalk)
	{
		return CHelicoScarrow::STATE::HELICO_START;
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_IDLE::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CHelico_IDLE::State_Exit()
{
}

CHelico_IDLE* CHelico_IDLE::Create(CGameObject* pGameObject)
{
	CHelico_IDLE* pInstance = new CHelico_IDLE();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_IDLE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_IDLE::Free()
{
	__super::Free();

}

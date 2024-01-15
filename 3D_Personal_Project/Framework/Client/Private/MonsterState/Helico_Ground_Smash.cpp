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
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);
}

_uint CHelico_Ground_Smash::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Ground_Smash::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_Ground_Smash::State_Late_Tick(_float fTimeDelta)
{

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

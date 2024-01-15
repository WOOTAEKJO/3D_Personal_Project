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
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);
}

_uint CHelico_Attack::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Attack::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_Attack::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CHelico_Attack::State_Exit()
{
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

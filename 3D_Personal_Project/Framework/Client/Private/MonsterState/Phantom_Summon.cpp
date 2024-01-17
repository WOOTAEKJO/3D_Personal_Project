#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Summon.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Summon::CPhantom_Summon()
{
}

HRESULT CPhantom_Summon::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Summon::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Summon::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Summon::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Summon::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Summon::State_Exit()
{
}

CPhantom_Summon* CPhantom_Summon::Create(CGameObject* pGameObject)
{
	CPhantom_Summon* pInstance = new CPhantom_Summon();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Summon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Summon::Free()
{
	__super::Free();

}

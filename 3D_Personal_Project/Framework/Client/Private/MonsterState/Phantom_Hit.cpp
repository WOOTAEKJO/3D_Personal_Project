#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Hit.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Hit::CPhantom_Hit()
{
}

HRESULT CPhantom_Hit::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Hit::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Hit::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Hit::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Hit::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Hit::State_Exit()
{
}

CPhantom_Hit* CPhantom_Hit::Create(CGameObject* pGameObject)
{
	CPhantom_Hit* pInstance = new CPhantom_Hit();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Hit");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Hit::Free()
{
	__super::Free();

}

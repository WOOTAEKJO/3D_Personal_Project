#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Appear.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Appear::CPhantom_Appear()
{
}

HRESULT CPhantom_Appear::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Appear::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Appear::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Appear::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Appear::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Appear::State_Exit()
{
}

CPhantom_Appear* CPhantom_Appear::Create(CGameObject* pGameObject)
{
	CPhantom_Appear* pInstance = new CPhantom_Appear();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Appear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Appear::Free()
{
	__super::Free();

}

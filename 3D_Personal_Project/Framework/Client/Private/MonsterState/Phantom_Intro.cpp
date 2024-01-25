#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Intro.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Intro::CPhantom_Intro()
{
}

HRESULT CPhantom_Intro::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Intro::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Intro::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Intro::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Intro::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Intro::State_Exit()
{
}

CPhantom_Intro* CPhantom_Intro::Create(CGameObject* pGameObject)
{
	CPhantom_Intro* pInstance = new CPhantom_Intro();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Intro");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Intro::Free()
{
	__super::Free();

}

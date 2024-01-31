#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_IntroEnd.h"
#include "StateMachine.h"

#include "Phantom.h"
#include "Trigger.h"

CPhantom_IntroEnd::CPhantom_IntroEnd()
{
}

HRESULT CPhantom_IntroEnd::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_IntroEnd::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::VANISH);

}

_uint CPhantom_IntroEnd::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_IntroEnd::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_IntroEnd::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CPhantom::STATE::IDLE;
	}

	return m_iStateID;
}

void CPhantom_IntroEnd::State_Exit()
{
	m_pOnwerTransform->Set_State(CTransform::STATE::STATE_POS, _float4(0.f, 0.f, 0.f, 1.f));
}

CPhantom_IntroEnd* CPhantom_IntroEnd::Create(CGameObject* pGameObject)
{
	CPhantom_IntroEnd* pInstance = new CPhantom_IntroEnd();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_IntroEnd");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_IntroEnd::Free()
{
	__super::Free();

}

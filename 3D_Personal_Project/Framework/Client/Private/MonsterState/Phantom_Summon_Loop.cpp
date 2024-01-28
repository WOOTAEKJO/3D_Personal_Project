#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Summon_Loop.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Summon_Loop::CPhantom_Summon_Loop()
{
}

HRESULT CPhantom_Summon_Loop::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Summon_Loop::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::SUMMON_LOOP);

}

_uint CPhantom_Summon_Loop::State_Priority_Tick(_float fTimeDelta)
{
	if (m_iCount > 30)
	{
		return CPhantom::STATE::VANISH;
	}

	return m_iStateID;
}

_uint CPhantom_Summon_Loop::State_Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime > 0.5f)
	{
		m_fTime = 0.f;
		dynamic_cast<CPhantom*>(m_pOwner)->Create_Meteor();

		m_iCount += 1;
	}
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Summon_Loop::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Summon_Loop::State_Exit()
{
	m_iCount = 0;
	m_fTime = 0.f;
}

CPhantom_Summon_Loop* CPhantom_Summon_Loop::Create(CGameObject* pGameObject)
{
	CPhantom_Summon_Loop* pInstance = new CPhantom_Summon_Loop();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Summon_Loop");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Summon_Loop::Free()
{
	__super::Free();

}

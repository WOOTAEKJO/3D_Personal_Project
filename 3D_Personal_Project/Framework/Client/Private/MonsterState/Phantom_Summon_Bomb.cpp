#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Summon_Bomb.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Summon_Bomb::CPhantom_Summon_Bomb()
{
}

HRESULT CPhantom_Summon_Bomb::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Summon_Bomb::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::SUMMON_BOMB);

	dynamic_cast<CPhantom*>(m_pOwner)->Create_Bomb();

}

_uint CPhantom_Summon_Bomb::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Summon_Bomb::State_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CPhantom::STATE::SUMMON_BOMB, 40.f))
	{
		if (m_bBomb)
		{
			dynamic_cast<CPhantom*>(m_pOwner)->Start_Point_Toward_Bomb();
			m_bBomb = false;
		}
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Summon_Bomb::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
		return CPhantom::STATE::SHOOT;

	return m_iStateID;
}

void CPhantom_Summon_Bomb::State_Exit()
{
	m_bBomb = true;
}

CPhantom_Summon_Bomb* CPhantom_Summon_Bomb::Create(CGameObject* pGameObject)
{
	CPhantom_Summon_Bomb* pInstance = new CPhantom_Summon_Bomb();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Summon_Bomb");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Summon_Bomb::Free()
{
	__super::Free();

}

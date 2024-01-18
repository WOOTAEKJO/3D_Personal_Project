#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Appear.h"
#include "StateMachine.h"

#include "Phantom.h"

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

	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::APPEAR);
}

_uint CPhantom_Appear::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Appear::State_Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_bTrans && m_fTime > 0.3f)
	{
		m_bTrans = false;
		if (dynamic_cast<CPhantom*>(m_pOwner)->Get_CurrentPhase() == CPhantom::PAHSE1)
		{
			dynamic_cast<CPhantom*>(m_pOwner)->Appear_OriginPos();
		}
		else
		{
			if (m_bSummonMeteor)
			{
				dynamic_cast<CPhantom*>(m_pOwner)->Appear_OriginPos();
			}
			else
			{
				dynamic_cast<CPhantom*>(m_pOwner)->Appear_PlayerPos();
			}
		}
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Appear::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		if (dynamic_cast<CPhantom*>(m_pOwner)->Get_CurrentPhase() == CPhantom::PAHSE1)
			return CPhantom::STATE::SUMMON_BOMB;
		else
		{
			if (m_bSummonMeteor)
			{
				m_bSummonMeteor = false;
				return CPhantom::STATE::SUMMON_BOMB;
			}
			else
			{
				m_bSummonMeteor = true;
				return CPhantom::STATE::SUMMON;
			}

		}
	}

	return m_iStateID;
}

void CPhantom_Appear::State_Exit()
{
	m_fTime = 0.f;
	m_bTrans = true;
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

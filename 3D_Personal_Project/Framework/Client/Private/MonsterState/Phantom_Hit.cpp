#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Hit.h"
#include "StateMachine.h"

#include "Phantom.h"

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
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::HIT);
	dynamic_cast<CPhantom*>(m_pOwner)->Add_Hit_Count();
}

_uint CPhantom_Hit::State_Priority_Tick(_float fTimeDelta)
{
	if(dynamic_cast<CPhantom*>(m_pOwner)->Judge_Hit())
		return CPhantom::STATE::DEAD;

	return m_iStateID;
}

_uint CPhantom_Hit::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Hit::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		/*if (dynamic_cast<CPhantom*>(m_pOwner)->Get_CurrentPhase() == CPhantom::PHASE::PAHSE1)
		{
			return CPhantom::STATE::MARTEAU;
		}
		else
		{

		}*/

		return CPhantom::STATE::MARTEAU;
	}

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

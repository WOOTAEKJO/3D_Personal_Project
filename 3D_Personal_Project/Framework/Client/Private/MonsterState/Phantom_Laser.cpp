#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Laser.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Laser::CPhantom_Laser()
{
}

HRESULT CPhantom_Laser::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Laser::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::LASER);
	dynamic_cast<CPhantom*>(m_pOwner)->Create_Laser();

}

_uint CPhantom_Laser::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Laser::State_Tick(_float fTimeDelta)
{
	m_pOwner->TargetLook_Y();
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Laser::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CPhantom::STATE::VANISH;
	}

	return m_iStateID;
}

void CPhantom_Laser::State_Exit()
{
	dynamic_cast<CPhantom*>(m_pOwner)->Delete_Laser();
}

CPhantom_Laser* CPhantom_Laser::Create(CGameObject* pGameObject)
{
	CPhantom_Laser* pInstance = new CPhantom_Laser();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Laser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Laser::Free()
{
	__super::Free();

}

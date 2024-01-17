#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Shoot.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Shoot::CPhantom_Shoot()
{
}

HRESULT CPhantom_Shoot::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Shoot::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Shoot::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Shoot::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Shoot::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Shoot::State_Exit()
{
}

CPhantom_Shoot* CPhantom_Shoot::Create(CGameObject* pGameObject)
{
	CPhantom_Shoot* pInstance = new CPhantom_Shoot();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Shoot");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Shoot::Free()
{
	__super::Free();

}

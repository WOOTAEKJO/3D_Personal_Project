#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Hit_Chasse.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Hit_Chasse::CPhantom_Hit_Chasse()
{
}

HRESULT CPhantom_Hit_Chasse::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Hit_Chasse::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Hit_Chasse::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Hit_Chasse::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Hit_Chasse::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Hit_Chasse::State_Exit()
{
}

CPhantom_Hit_Chasse* CPhantom_Hit_Chasse::Create(CGameObject* pGameObject)
{
	CPhantom_Hit_Chasse* pInstance = new CPhantom_Hit_Chasse();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Hit_Chasse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Hit_Chasse::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Attaque_Chasse.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Attaque_Chasse::CPhantom_Attaque_Chasse()
{
}

HRESULT CPhantom_Attaque_Chasse::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Attaque_Chasse::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Attaque_Chasse::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Attaque_Chasse::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Attaque_Chasse::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Attaque_Chasse::State_Exit()
{
}

CPhantom_Attaque_Chasse* CPhantom_Attaque_Chasse::Create(CGameObject* pGameObject)
{
	CPhantom_Attaque_Chasse* pInstance = new CPhantom_Attaque_Chasse();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Attaque_Chasse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Attaque_Chasse::Free()
{
	__super::Free();

}

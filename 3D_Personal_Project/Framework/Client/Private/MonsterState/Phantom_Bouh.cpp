#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Bouh.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CPhantom_Bouh::CPhantom_Bouh()
{
}

HRESULT CPhantom_Bouh::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Bouh::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);

}

_uint CPhantom_Bouh::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Bouh::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Bouh::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Bouh::State_Exit()
{
}

CPhantom_Bouh* CPhantom_Bouh::Create(CGameObject* pGameObject)
{
	CPhantom_Bouh* pInstance = new CPhantom_Bouh();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Bouh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Bouh::Free()
{
	__super::Free();

}

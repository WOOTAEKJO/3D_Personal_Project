#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Vanish.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Vanish::CPhantom_Vanish()
{
}

HRESULT CPhantom_Vanish::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Vanish::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::VANISH);

}

_uint CPhantom_Vanish::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Vanish::State_Tick(_float fTimeDelta)
{
	
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Vanish::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CPhantom::STATE::DASH;
	}

	return m_iStateID;
}

void CPhantom_Vanish::State_Exit()
{
}

CPhantom_Vanish* CPhantom_Vanish::Create(CGameObject* pGameObject)
{
	CPhantom_Vanish* pInstance = new CPhantom_Vanish();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Vanish");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Vanish::Free()
{
	__super::Free();

}

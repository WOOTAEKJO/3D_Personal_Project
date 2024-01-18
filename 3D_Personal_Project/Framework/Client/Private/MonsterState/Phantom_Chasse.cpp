#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Chasse.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Chasse::CPhantom_Chasse()
{
}

HRESULT CPhantom_Chasse::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Chasse::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::CHASSE);

}

_uint CPhantom_Chasse::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Chasse::State_Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > 1.5f)
	{
		return CPhantom::STATE::MARTEAU;
	}

	m_pOwner->TargetLook();
	
	Translate_Simple(CTransform::STATE::STATE_LOOK,2.f, fTimeDelta);

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Chasse::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Chasse::State_Exit()
{
	m_fTimeAcc = 0.f;
}

CPhantom_Chasse* CPhantom_Chasse::Create(CGameObject* pGameObject)
{
	CPhantom_Chasse* pInstance = new CPhantom_Chasse();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Chasse");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Chasse::Free()
{
	__super::Free();

}

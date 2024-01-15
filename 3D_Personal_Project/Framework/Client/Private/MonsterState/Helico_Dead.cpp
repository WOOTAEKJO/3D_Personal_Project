#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Dead.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Dead::CHelico_Dead()
{
}

HRESULT CHelico_Dead::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Dead::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);
}

_uint CHelico_Dead::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Dead::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_Dead::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CHelico_Dead::State_Exit()
{
}

CHelico_Dead* CHelico_Dead::Create(CGameObject* pGameObject)
{
	CHelico_Dead* pInstance = new CHelico_Dead();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Dead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Dead::Free()
{
	__super::Free();

}

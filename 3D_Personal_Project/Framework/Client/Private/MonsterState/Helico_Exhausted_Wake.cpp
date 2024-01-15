#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Exhausted_Wake.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Exhausted_Wake::CHelico_Exhausted_Wake()
{
}

HRESULT CHelico_Exhausted_Wake::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Exhausted_Wake::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);
}

_uint CHelico_Exhausted_Wake::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Exhausted_Wake::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_Exhausted_Wake::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CHelico_Exhausted_Wake::State_Exit()
{
}

CHelico_Exhausted_Wake* CHelico_Exhausted_Wake::Create(CGameObject* pGameObject)
{
	CHelico_Exhausted_Wake* pInstance = new CHelico_Exhausted_Wake();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Exhausted_Wake");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Exhausted_Wake::Free()
{
	__super::Free();

}

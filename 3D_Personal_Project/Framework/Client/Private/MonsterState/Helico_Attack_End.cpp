#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Attack_End.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Attack_End::CHelico_Attack_End()
{
}

HRESULT CHelico_Attack_End::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Attack_End::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::HELICO_END);
}

_uint CHelico_Attack_End::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Attack_End::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CHelico_Attack_End::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
		return CHelicoScarrow::STATE::EXHAUSTED_IDLE;

	return m_iStateID;
}

void CHelico_Attack_End::State_Exit()
{
}

CHelico_Attack_End* CHelico_Attack_End::Create(CGameObject* pGameObject)
{
	CHelico_Attack_End* pInstance = new CHelico_Attack_End();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Attack_End");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Attack_End::Free()
{
	__super::Free();

}

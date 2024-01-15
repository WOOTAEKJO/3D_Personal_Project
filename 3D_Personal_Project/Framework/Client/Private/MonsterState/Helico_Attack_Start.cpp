#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Attack_Start.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Attack_Start::CHelico_Attack_Start()
{
}

HRESULT CHelico_Attack_Start::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Attack_Start::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::HELICO_START);
}

_uint CHelico_Attack_Start::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Attack_Start::State_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
		return CHelicoScarrow::STATE::HELICO_ATTACK;

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CHelico_Attack_Start::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CHelico_Attack_Start::State_Exit()
{
}

CHelico_Attack_Start* CHelico_Attack_Start::Create(CGameObject* pGameObject)
{
	CHelico_Attack_Start* pInstance = new CHelico_Attack_Start();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Attack_Start");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Attack_Start::Free()
{
	__super::Free();

}

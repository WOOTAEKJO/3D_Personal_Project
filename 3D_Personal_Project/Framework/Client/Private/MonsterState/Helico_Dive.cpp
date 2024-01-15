#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Dive.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Dive::CHelico_Dive()
{
}

HRESULT CHelico_Dive::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Dive::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::DIVE);
}

_uint CHelico_Dive::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Dive::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CHelico_Dive::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CHelicoScarrow::STATE::DIVE_IDLE;
	}

	return m_iStateID;
}

void CHelico_Dive::State_Exit()
{
	dynamic_cast<CHelicoScarrow*>(m_pOwner)->Create_Monster();
}

CHelico_Dive* CHelico_Dive::Create(CGameObject* pGameObject)
{
	CHelico_Dive* pInstance = new CHelico_Dive();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Dive");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Dive::Free()
{
	__super::Free();

}

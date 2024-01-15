#include "stdafx.h"
#include "..\Public\MonsterState\Helico_Dive_Up.h"
#include "StateMachine.h"

#include "HelicoScarrow.h"

CHelico_Dive_Up::CHelico_Dive_Up()
{
}

HRESULT CHelico_Dive_Up::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CHelico_Dive_Up::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CHelicoScarrow::STATE::IDLE);
}

_uint CHelico_Dive_Up::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CHelico_Dive_Up::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CHelico_Dive_Up::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CHelico_Dive_Up::State_Exit()
{
}

CHelico_Dive_Up* CHelico_Dive_Up::Create(CGameObject* pGameObject)
{
	CHelico_Dive_Up* pInstance = new CHelico_Dive_Up();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Dive_Up");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Dive_Up::Free()
{
	__super::Free();

}

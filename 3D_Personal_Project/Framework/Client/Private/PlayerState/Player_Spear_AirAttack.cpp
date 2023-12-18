#include "stdafx.h"
#include "..\Public\PlayerState\Player_Spear_AirAttack.h"

#include "GameInstance.h"

CPlayer_Spear_AirAttack::CPlayer_Spear_AirAttack()
{
}

HRESULT CPlayer_Spear_AirAttack::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Spear_AirAttack::State_Enter()
{
	m_pOwner->Get_ModelCom()->Set_AnimationIndex(76);
}

_uint CPlayer_Spear_AirAttack::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Spear_AirAttack::State_Tick(_float fTimeDelta)
{
	m_pOwner->Get_ModelCom()->Play_Animation(fTimeDelta, false);

	if (m_pOwner->Get_ModelCom()->Is_Animation_Finished())
		return CPlayer::STATE::IDLE;

	return m_iStateID;
}

_uint CPlayer_Spear_AirAttack::State_Late_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

void CPlayer_Spear_AirAttack::State_Exit()
{
	
}

void CPlayer_Spear_AirAttack::Key_Input()
{
}

CPlayer_Spear_AirAttack* CPlayer_Spear_AirAttack::Create(CGameObject* pGameObject)
{
	CPlayer_Spear_AirAttack* pInstance = new CPlayer_Spear_AirAttack();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Spear_AirAttack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Spear_AirAttack::Free()
{
	__super::Free();
}

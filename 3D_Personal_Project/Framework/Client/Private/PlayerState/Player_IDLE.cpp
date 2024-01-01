#include "stdafx.h"
#include "..\Public\PlayerState\Player_IDLE.h"

#include "GameInstance.h"

CPlayer_IDLE::CPlayer_IDLE()
{
}

HRESULT CPlayer_IDLE::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_IDLE::State_Enter()
{
	m_pOwner->Animation_By_Type(CPlayer::STATE::IDLE);
}

_uint CPlayer_IDLE::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPlayer_IDLE::State_Tick(_float fTimeDelta)
{

	if (m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
		return m_pOwner->Get_NextAttackID();

	if(m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_FRONT))
		return CPlayer::STATE::RUN;
	if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_BACK))
		return CPlayer::STATE::RUN;
	if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_RIGHT))
		return CPlayer::STATE::RUN;
	if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_LEFT))
		return CPlayer::STATE::RUN;

	if (m_pOnwerController->Key_Down(CPlayer::KEY_STATE::KEY_JUMP))
		return CPlayer::STATE::JUMP;

	if (m_pOnwerController->Key_Down(CPlayer::KEY_STATE::KEY_ROLL))
		return CPlayer::STATE::ROLL;

	m_pOwner->Get_BodyModel()->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPlayer_IDLE::State_Late_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

void CPlayer_IDLE::State_Exit()
{
}

CPlayer_IDLE* CPlayer_IDLE::Create(CGameObject* pGameObject)
{
	CPlayer_IDLE* pInstance = new CPlayer_IDLE();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_IDLE");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_IDLE::Free()
{
	__super::Free();
}

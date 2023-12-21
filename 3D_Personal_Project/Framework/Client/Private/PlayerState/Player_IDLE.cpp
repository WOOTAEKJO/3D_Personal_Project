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
	m_pOwnerModel->Set_AnimationIndex(83);
}

_uint CPlayer_IDLE::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CPlayer_IDLE::State_Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing(DIK_UP))
	{
		return CPlayer::STATE::RUN;
	}

	if (m_pGameInstance->Key_Down(DIK_Q))
	{
		return CPlayer::STATE::ATTACK1;
	}

	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		return CPlayer::STATE::JUMP;
	}

	if (m_pGameInstance->Key_Down(DIK_LSHIFT))
	{
		return CPlayer::STATE::ROLL;
	}

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

#include "stdafx.h"
#include "..\Public\PlayerState\Player_Run.h"

#include "GameInstance.h"

CPlayer_Run::CPlayer_Run()
{
}

HRESULT CPlayer_Run::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Run::State_Enter()
{
	m_pOwner->Get_ModelCom()->Set_AnimationIndex(86);
}

_uint CPlayer_Run::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Run::State_Tick(_float fTimeDelta)
{
	if (!m_pGameInstance->Key_Pressing(DIK_UP))
	{
		return CPlayer::STATE::IDLE;
	}

	if (m_pGameInstance->Key_Down(DIK_Q))
	{
		return CPlayer::STATE::ATTACK;
	}

	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		//m_pOwner->Get_RigidBody()->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), 20.f, fTimeDelta);
		m_pOwner->Get_RigidBody()->Force(m_pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_UP), 20.f, fTimeDelta);
		m_pOwner->Get_RigidBody()->Set_Jump();
		return CPlayer::STATE::JUMP;
	}

	if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	else if(m_pGameInstance->Key_Pressing(DIK_LEFT))
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -1.f*fTimeDelta);

	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());

	m_pOwner->Get_ModelCom()->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPlayer_Run::State_Late_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

void CPlayer_Run::State_Exit()
{
	
}

void CPlayer_Run::Key_Input()
{
}

CPlayer_Run* CPlayer_Run::Create(CGameObject* pGameObject)
{
	CPlayer_Run* pInstance = new CPlayer_Run();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Run");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Run::Free()
{
	__super::Free();
}

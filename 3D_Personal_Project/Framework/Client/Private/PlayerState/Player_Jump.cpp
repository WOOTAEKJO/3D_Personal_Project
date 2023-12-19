#include "stdafx.h"
#include "..\Public\PlayerState\Player_Jump.h"

#include "GameInstance.h"

CPlayer_Jump::CPlayer_Jump()
{
}

HRESULT CPlayer_Jump::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Jump::State_Enter()
{
	m_pOwner->Get_ModelCom()->Set_AnimationIndex(84);

	m_pOwner->Open_Physics_Desc()->bGround = false;
	m_pOwner->Open_Physics_Desc()->bJump = true;

	Jump();
}

_uint CPlayer_Jump::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Jump::State_Tick(_float fTimeDelta)
{
	Move(fTimeDelta);

	m_pOwner->Get_ModelCom()->Play_Animation(fTimeDelta, false);

	/*if (m_pOwner->Get_ModelCom()->Is_Animation_Finished())
		return CPlayer::STATE::IDLE;*/

	return m_iStateID;
}

_uint CPlayer_Jump::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwner->Open_Physics_Desc()->bFall && !m_pOwner->Open_Physics_Desc()->bLanding
		&& m_pOwner->Get_RigidBody()->Is_Land())
	{
		Land();
		return CPlayer::STATE::IDLE;
	}

	if (!m_pOwner->Open_Physics_Desc()->bFall)
	{
		Fall();
	}

	return m_iStateID;
}

void CPlayer_Jump::State_Exit()
{
	m_pOwner->Open_Physics_Desc()->bGround = true;
	m_pOwner->Open_Physics_Desc()->bLanding = false;
}

void CPlayer_Jump::Jump()
{
	m_pOwner->Get_RigidBody()->Jump(m_pOwner->Open_Physics_Desc()->fJumpPower, m_pOwner->Open_Physics_Desc()->fJumpGravity);
}

void CPlayer_Jump::Land()
{
	m_pOwner->Get_RigidBody()->Land();

	m_pOwner->Open_Physics_Desc()->bDoubleJump = false;
	m_pOwner->Open_Physics_Desc()->bFall = false;
	m_pOwner->Open_Physics_Desc()->bJump = false;
	m_pOwner->Open_Physics_Desc()->bLanding = true;
}

void CPlayer_Jump::Fall()
{
	if (m_pOwner->Get_RigidBody()->Is_Power_Zero(CRigidBody::TYPE::TYPE_VELOCITY)) {
		m_pOwner->Open_Physics_Desc()->bFall = true;
		m_pOwner->Get_RigidBody()->Set_GravityPower(m_pOwner->Open_Physics_Desc()->fFallGravity);
	}
}

void CPlayer_Jump::Move(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing(DIK_UP))
	{
		_vector vDir = m_pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_LOOK);
		_vector vPos = XMVector3Normalize(vDir) * m_pOwner->Open_Physics_Desc()->fForwardSpeed * fTimeDelta;
		m_pOwner->Get_Transform()->Translate(vPos,m_pOwner->Get_Navigation());
	}
	else if (m_pGameInstance->Key_Pressing(DIK_DOWN))
	{
		_vector vDir = m_pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_LOOK);
		_vector vPos = XMVector3Normalize(vDir) * m_pOwner->Open_Physics_Desc()->fForwardSpeed * fTimeDelta * -1.f;
		m_pOwner->Get_Transform()->Translate(vPos, m_pOwner->Get_Navigation());
	}
	else if (m_pGameInstance->Key_Pressing(DIK_LEFT))
	{
		_vector vDir = m_pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_RIGHT);
		_vector vPos = XMVector3Normalize(vDir) * m_pOwner->Open_Physics_Desc()->fForwardSpeed * fTimeDelta * -1.f;
		m_pOwner->Get_Transform()->Translate(vPos, m_pOwner->Get_Navigation());
	}
	else if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
	{
		_vector vDir = m_pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_RIGHT);
		_vector vPos = XMVector3Normalize(vDir) * m_pOwner->Open_Physics_Desc()->fForwardSpeed * fTimeDelta;
		m_pOwner->Get_Transform()->Translate(vPos, m_pOwner->Get_Navigation());
	}
}

CPlayer_Jump* CPlayer_Jump::Create(CGameObject* pGameObject)
{
	CPlayer_Jump* pInstance = new CPlayer_Jump();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Jump");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Jump::Free()
{
	__super::Free();
}

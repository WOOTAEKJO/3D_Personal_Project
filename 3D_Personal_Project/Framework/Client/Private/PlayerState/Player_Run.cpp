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
	m_pOwnerModel->Set_AnimationIndex(86);
}

_uint CPlayer_Run::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Run::State_Tick(_float fTimeDelta)
{
	if (!m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_FRONT))
		return CPlayer::STATE::IDLE;

	if (m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
		return CPlayer::STATE::ATTACK2;

	if (m_pOnwerController->Key_Down(CPlayer::KEY_STATE::KEY_ROLL))
		return CPlayer::STATE::ROLL;

	if (m_pGameInstance->Key_Pressing(DIK_D))
		m_pOnwerTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	else if(m_pGameInstance->Key_Pressing(DIK_A))
		m_pOnwerTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -1.f*fTimeDelta);

	m_pOnwerTransform->Go_Straight(fTimeDelta, m_pOnwerNavigation);

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPlayer_Run::State_Late_Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_SPACE))
	{
		//m_pOwner->Get_RigidBody()->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), 20.f, fTimeDelta);
		/*m_pOwner->Get_RigidBody()->Force(m_pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_UP), 20.f, fTimeDelta);
		m_pOwner->Get_RigidBody()->Set_Jump();*/
		return CPlayer::STATE::JUMP;
	}

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

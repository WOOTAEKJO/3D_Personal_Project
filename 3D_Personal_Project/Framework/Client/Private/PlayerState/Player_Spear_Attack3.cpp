#include "stdafx.h"
#include "..\Public\PlayerState\Player_Spear_Attack3.h"

#include "GameInstance.h"

CPlayer_Spear_Attack3::CPlayer_Spear_Attack3()
{
}

HRESULT CPlayer_Spear_Attack3::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Spear_Attack3::State_Enter()
{
	Trans_Attack(true);
	//m_pOwnerModel->Set_AnimationIndex(81);
	m_pOwner->Animation_By_Type(CPlayer::STATE::ATTACK3);
}

_uint CPlayer_Spear_Attack3::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Spear_Attack3::State_Tick(_float fTimeDelta)
{

	m_fTime += fTimeDelta;

	if (m_fTime > m_fAttackDelay)
	{
		if (!m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
			return CPlayer::STATE::IDLE;
		else {
			return m_pOwner->Get_NextAttackID();
		}
	}
	if (m_pOwnerModel->Is_Animation_Finished())
		return CPlayer::STATE::IDLE;

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	if(m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_FRONT))
		Shovel_Rush_Attack(fTimeDelta);

	return m_iStateID;
}

_uint CPlayer_Spear_Attack3::State_Late_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

void CPlayer_Spear_Attack3::State_Exit()
{
	m_fTime = 0.f;
	Trans_Attack(false);
}

void CPlayer_Spear_Attack3::Shovel_Rush_Attack(_float fTimeDelta)
{
	Translate(CTransform::STATE::STATE_LOOK, 12.f, fTimeDelta);
}

CPlayer_Spear_Attack3* CPlayer_Spear_Attack3::Create(CGameObject* pGameObject)
{
	CPlayer_Spear_Attack3* pInstance = new CPlayer_Spear_Attack3();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Spear_Attack3");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Spear_Attack3::Free()
{
	__super::Free();
}

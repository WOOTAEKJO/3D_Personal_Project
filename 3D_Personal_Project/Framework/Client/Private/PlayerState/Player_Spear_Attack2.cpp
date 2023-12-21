#include "stdafx.h"
#include "..\Public\PlayerState\Player_Spear_Attack2.h"

#include "GameInstance.h"

CPlayer_Spear_Attack2::CPlayer_Spear_Attack2()
{
}

HRESULT CPlayer_Spear_Attack2::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Spear_Attack2::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(79);
}

_uint CPlayer_Spear_Attack2::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Spear_Attack2::State_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_Animation_Finished()) {

		m_fTime += fTimeDelta;

		if (m_pGameInstance->Key_Down(DIK_Q))
		{
			m_fTime = 0.f;
			return CPlayer::STATE::ATTACK3;
		}

		if (m_fTime > 0.3f)
		{
			m_fTime = 0.f;
			return CPlayer::STATE::IDLE;
		}
	}
	else {
		/*if (m_pGameInstance->Key_Pressing(DIK_UP)) {
			Translate(CTransform::STATE::STATE_LOOK, 1.f, fTimeDelta);
		}*/
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, false);
	return m_iStateID;
}

_uint CPlayer_Spear_Attack2::State_Late_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

void CPlayer_Spear_Attack2::State_Exit()
{
	
}

void CPlayer_Spear_Attack2::Key_Input()
{
}

CPlayer_Spear_Attack2* CPlayer_Spear_Attack2::Create(CGameObject* pGameObject)
{
	CPlayer_Spear_Attack2* pInstance = new CPlayer_Spear_Attack2();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Spear_Attack2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Spear_Attack2::Free()
{
	__super::Free();
}

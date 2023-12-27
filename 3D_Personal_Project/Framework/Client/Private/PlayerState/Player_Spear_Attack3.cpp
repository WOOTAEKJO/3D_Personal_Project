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
	m_pOwnerModel->Set_AnimationIndex(81);
}

_uint CPlayer_Spear_Attack3::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Spear_Attack3::State_Tick(_float fTimeDelta)
{
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	if (m_pOwnerModel->Is_Animation_Finished())
		return CPlayer::STATE::IDLE;

	return m_iStateID;
}

_uint CPlayer_Spear_Attack3::State_Late_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

void CPlayer_Spear_Attack3::State_Exit()
{
	Trans_Attack(false);
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

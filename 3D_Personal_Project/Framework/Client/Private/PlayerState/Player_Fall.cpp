#include "stdafx.h"
#include "..\Public\PlayerState\Player_Fall.h"

#include "GameInstance.h"

CPlayer_Fall::CPlayer_Fall()
{
}

HRESULT CPlayer_Fall::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Fall::State_Enter()
{
	m_pOwner->Animation_By_Type(CPlayer::STATE::LAND);
}

_uint CPlayer_Fall::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPlayer_Fall::State_Tick(_float fTimeDelta)
{

	m_pOwner->Get_BodyModel()->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPlayer_Fall::State_Late_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

void CPlayer_Fall::State_Exit()
{
}

CPlayer_Fall* CPlayer_Fall::Create(CGameObject* pGameObject)
{
	CPlayer_Fall* pInstance = new CPlayer_Fall();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Fall");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Fall::Free()
{
	__super::Free();
}

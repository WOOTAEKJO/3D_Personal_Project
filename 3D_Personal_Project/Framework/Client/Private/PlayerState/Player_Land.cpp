#include "stdafx.h"
#include "..\Public\PlayerState\Player_Land.h"

#include "GameInstance.h"

CPlayer_Land::CPlayer_Land()
{
}

HRESULT CPlayer_Land::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Land::State_Enter()
{
	m_pOwner->Animation_By_Type(CPlayer::STATE::LAND);
}

_uint CPlayer_Land::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPlayer_Land::State_Tick(_float fTimeDelta)
{

	if (m_pOwnerModel->Is_Animation_Finished())
		return CPlayer::STATE::IDLE;

	m_pOwner->Get_BodyModel()->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPlayer_Land::State_Late_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

void CPlayer_Land::State_Exit()
{
}

CPlayer_Land* CPlayer_Land::Create(CGameObject* pGameObject)
{
	CPlayer_Land* pInstance = new CPlayer_Land();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Land");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Land::Free()
{
	__super::Free();
}

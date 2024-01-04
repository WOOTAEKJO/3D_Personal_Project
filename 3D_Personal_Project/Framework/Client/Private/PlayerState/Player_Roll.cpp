#include "stdafx.h"
#include "..\Public\PlayerState\Player_Roll.h"

#include "GameInstance.h"

CPlayer_Roll::CPlayer_Roll()
{
}

HRESULT CPlayer_Roll::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CPlayer_State::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Roll::State_Enter()
{
	//m_pOwnerModel->Set_AnimationIndex(114);
	m_pOwner->Animation_By_Type(CPlayer::STATE::ROLL);
}

_uint CPlayer_Roll::State_Priority_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CPlayer_Roll::State_Tick(_float fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CPlayer::STATE::IDLE;	
	}

	Translate(CTransform::STATE::STATE_LOOK, 10.f, fTimeDelta);

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPlayer_Roll::State_Late_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

void CPlayer_Roll::State_Exit()
{
	
}

CPlayer_Roll* CPlayer_Roll::Create(CGameObject* pGameObject)
{
	CPlayer_Roll* pInstance = new CPlayer_Roll();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Roll");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Roll::Free()
{
	__super::Free();
}

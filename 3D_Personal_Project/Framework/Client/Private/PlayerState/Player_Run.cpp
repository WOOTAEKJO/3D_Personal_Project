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
	m_pOwner->Animation_By_Type(CPlayer::STATE::RUN);

}

_uint CPlayer_Run::State_Priority_Tick(_float fTimeDelta)
{
	/*if (Falling())
		return CPlayer::STATE::FALL;*/

	return m_iStateID;
}

_uint CPlayer_Run::State_Tick(_float fTimeDelta)
{

	if (m_pOnwerController->Key_Down(CPlayer::KEY_STATE::KEY_ROLL))
		return CPlayer::STATE::ROLL;

	if (m_pOnwerController->Key_Down(CPlayer::KEY_STATE::KEY_JUMP))
		return CPlayer::STATE::JUMP;

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPlayer_Run::State_Late_Tick(_float fTimeDelta)
{
	if (!Key_Input(fTimeDelta))
		return CPlayer::STATE::IDLE;

	Translate(CTransform::STATE::STATE_LOOK, m_pOwner->Open_Physics_Desc()->fForwardSpeed, fTimeDelta);

	if (m_pOnwerController->Mouse_Down(CPlayer::KEY_STATE::KEY_LB_ATTACK))
		return m_pOwner->Get_NextAttackID();

	return m_iStateID;
}

void CPlayer_Run::State_Exit()
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

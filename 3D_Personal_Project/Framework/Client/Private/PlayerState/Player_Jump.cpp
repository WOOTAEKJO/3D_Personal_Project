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
}

_uint CPlayer_Jump::State_Priority_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

_uint CPlayer_Jump::State_Tick(_float fTimeDelta)
{
	m_pOwner->Get_ModelCom()->Play_Animation(fTimeDelta, false);

	if (m_pOwner->Get_ModelCom()->Is_Animation_Finished())
		return CPlayer::STATE::IDLE;

	return m_iStateID;
}

_uint CPlayer_Jump::State_Late_Tick(_float fTimeDelta)
{
	

	return m_iStateID;
}

void CPlayer_Jump::State_Exit()
{
	
}

void CPlayer_Jump::Key_Input()
{
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

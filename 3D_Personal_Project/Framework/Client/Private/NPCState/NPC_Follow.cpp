#include "stdafx.h"
#include "..\Public\NPCState\NPC_Follow.h"

#include "Crow.h"

CNPC_Follow::CNPC_Follow()
{
}

HRESULT CNPC_Follow::Initialize(CGameObject* pGameObject)
{
	if (FAILED(__super::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNPC_Follow::State_Enter()
{
	m_pOwner->Set_TypeAnimIndex(CNPC::STATE::FOLLOW);
}

_uint CNPC_Follow::State_Priority_Tick(_float fTimeDelta)
{
	_float fSpeed = m_pOwner->Distance_Proportional(0.1f, 7.f, 2.f);
	m_pOwner->Target_Follow(fTimeDelta * fSpeed);
	return m_iStateID;
}

_uint CNPC_Follow::State_Tick(_float fTimeDelta)
{
	if (dynamic_cast<CCrow*>(m_pOwner)->Attack_Input())
		return CNPC::STATE::ATTACK;

	if (m_pOwner->Is_Target_Range(0.02f))
	{
		return CNPC::STATE::IDLE;
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, true);
	return m_iStateID;
}

_uint CNPC_Follow::State_Late_Tick(_float fTimeDelta)
{

	if(m_pOwner->Turn(fTimeDelta))
		m_pOwner->Target_Follow_Look();

	return m_iStateID;
}

void CNPC_Follow::State_Exit()
{
	m_fTime = 0.f;
}

CNPC_Follow* CNPC_Follow::Create(CGameObject* pGameObject)
{
	CNPC_Follow* pInstance = new CNPC_Follow();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNPC_Follow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNPC_Follow::Free()
{
	__super::Free();
}

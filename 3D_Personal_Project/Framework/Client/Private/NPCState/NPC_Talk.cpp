#include "stdafx.h"
#include "..\Public\NPCState\NPC_Talk.h"

#include "Player.h"
#include "Crow.h"

CNPC_Talk::CNPC_Talk()
{
}

HRESULT CNPC_Talk::Initialize(CGameObject* pGameObject)
{
	if (FAILED(__super::Initialize(pGameObject)))
		return E_FAIL;

	return S_OK;
}

void CNPC_Talk::State_Enter()
{
	m_pOwner->Set_TypeAnimIndex(CNPC::STATE::TALK);
}

_uint CNPC_Talk::State_Priority_Tick(_float fTimeDelta)
{
	
	return m_iStateID;
}

_uint CNPC_Talk::State_Tick(_float fTimeDelta)
{
	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CNPC_Talk::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CNPC_Talk::State_Exit()
{
}

CNPC_Talk* CNPC_Talk::Create(CGameObject* pGameObject)
{
	CNPC_Talk* pInstance = new CNPC_Talk();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CNPC_Talk");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNPC_Talk::Free()
{
	__super::Free();
}

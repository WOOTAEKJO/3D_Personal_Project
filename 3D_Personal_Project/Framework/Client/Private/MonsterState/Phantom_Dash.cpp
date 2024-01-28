#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Dash.h"
#include "StateMachine.h"

#include "Phantom.h"

#include "Effect_DashRoad.h"

CPhantom_Dash::CPhantom_Dash()
{
}

HRESULT CPhantom_Dash::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	

	return S_OK;
}

void CPhantom_Dash::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::CHASSE);

	m_iCount += 1;

	m_pOnwerTransform->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	dynamic_cast<CPhantom*>(m_pOwner)->Create_Multiply();

}

_uint CPhantom_Dash::State_Priority_Tick(_float fTimeDelta)
{
	if (m_iCount >= 10)
	{
		m_fChangeTime += fTimeDelta;
		if (m_fChangeTime > 1.f)
		{
			return CPhantom::STATE::APPEAR;
		}
	}

	return m_iStateID;
}

_uint CPhantom_Dash::State_Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime > 0.8f)
	{
		if (m_iCount < 10)
		{
			m_fTime = 0.f;
			m_iCount += 1;
			dynamic_cast<CPhantom*>(m_pOwner)->Create_Multiply();
		}
	}

	m_pOwnerModel->Play_Animation(fTimeDelta, true);

	return m_iStateID;
}

_uint CPhantom_Dash::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPhantom_Dash::State_Exit()
{
	m_fChangeTime = 0.f;
	m_fTime = 0.f;
	m_iCount = 0;

}

CPhantom_Dash* CPhantom_Dash::Create(CGameObject* pGameObject)
{
	CPhantom_Dash* pInstance = new CPhantom_Dash();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Dash");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Dash::Free()
{
	__super::Free();

}

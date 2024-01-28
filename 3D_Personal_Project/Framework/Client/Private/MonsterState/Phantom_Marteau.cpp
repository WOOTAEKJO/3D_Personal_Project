#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Marteau.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Marteau::CPhantom_Marteau()
{
}

HRESULT CPhantom_Marteau::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Marteau::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::MARTEAU);

	dynamic_cast<CPhantom*>(m_pOwner)->Adjust_Pos(_float3(0.f, 0.5f, 0.f));

	m_iCount += 1;

	
	dynamic_cast<CPhantom*>(m_pOwner)->Set_SmashTime(true);
}

_uint CPhantom_Marteau::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Marteau::State_Tick(_float fTimeDelta)
{

	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Marteau::State_Late_Tick(_float fTimeDelta)
{
	_float fDuration = m_pOwnerModel->CurAnim_Get_Duration(CPhantom::STATE::MARTEAU);
	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CPhantom::STATE::MARTEAU, fDuration - 80.f))
	{
		if (m_bAttack)
		{
			if (m_iCount == 1 && dynamic_cast<CPhantom*>(m_pOwner)->Get_CurrentPhase() == CPhantom::PHASE::PAHSE2)
			{
				_uint iNum = 0;
				if (dynamic_cast<CPhantom*>(m_pOwner)->Get_Hit_Count() == 3)
					iNum = 1;
				dynamic_cast<CPhantom*>(m_pOwner)->Drop_Floor(iNum);
				dynamic_cast<CPhantom*>(m_pOwner)->Navi_Filter();
			}
			dynamic_cast<CPhantom*>(m_pOwner)->Create_Shock_Wave();
			m_bAttack = false;
		}

	}

	if (m_pOwnerModel->Is_Animation_Finished())
	{
		if (m_iCount >= 5)
		{
			m_iCount = 0;
			return CPhantom::STATE::LASER;
		}
		else {
			return CPhantom::STATE::CHASSE;
		}
	}

	return m_iStateID;
}

void CPhantom_Marteau::State_Exit()
{
	m_bAttack = true;
	dynamic_cast<CPhantom*>(m_pOwner)->Adjust_Pos(_float3(0.f, -0.5f, 0.f));

	dynamic_cast<CPhantom*>(m_pOwner)->Set_SmashTime(false);
}

CPhantom_Marteau* CPhantom_Marteau::Create(CGameObject* pGameObject)
{
	CPhantom_Marteau* pInstance = new CPhantom_Marteau();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Marteau");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Marteau::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MonsterState\Phantom_Laser.h"
#include "StateMachine.h"

#include "Phantom.h"

CPhantom_Laser::CPhantom_Laser()
{
}

HRESULT CPhantom_Laser::Initialize(CGameObject* pGameObject)
{
	if (FAILED(CMonster_State::Initialize(pGameObject)))
		return E_FAIL;
	
	return S_OK;
}

void CPhantom_Laser::State_Enter()
{
	m_pOwnerModel->Set_AnimationIndex(CPhantom::STATE::LASER);
	
	m_pGameInstance->Play_Sound(L"Phantom", L"LaserVoice.ogg", CHANNELID::SOUND_BOSS_VOICE, 0.7f);
}

_uint CPhantom_Laser::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPhantom_Laser::State_Tick(_float fTimeDelta)
{
	_float fDuration = m_pOwnerModel->CurAnim_Get_Duration(CPhantom::STATE::LASER);
	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CPhantom::STATE::LASER, 40.f))
	{

		if (m_bAttack)
		{
			dynamic_cast<CPhantom*>(m_pOwner)->Create_Laser();
			m_pGameInstance->Play_Sound(L"Phantom", L"Laser.ogg", CHANNELID::SOUND_BOSS_ATTACK, 0.7f);
			m_pGameInstance->Play_Sound(L"Phantom", L"LaserFire.ogg", CHANNELID::SOUND_BOSS_HIT, 1.f, true);
			m_bAttack = false;
		}
		else {
			if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CPhantom::STATE::LASER, 250.f))
			{
				dynamic_cast<CPhantom*>(m_pOwner)->Delete_Laser();
			}
		}
	}

	

	m_pOwner->TargetLook();
	m_pOwnerModel->Play_Animation(fTimeDelta, false);

	return m_iStateID;
}

_uint CPhantom_Laser::State_Late_Tick(_float fTimeDelta)
{
	if (m_pOwnerModel->Is_CurAnim_Arrival_TrackPosition(CPhantom::STATE::LASER, 260.f))
	{
		if (m_bSound)
		{
			m_pGameInstance->Stop_Sound(CHANNELID::SOUND_BOSS_ATTACK);
			m_bSound = false;
		}
	}

	if (m_pOwnerModel->Is_Animation_Finished())
	{
		return CPhantom::STATE::VANISH;
	}

	return m_iStateID;
}

void CPhantom_Laser::State_Exit()
{

	m_pGameInstance->Stop_Sound(CHANNELID::SOUND_BOSS_HIT);
	m_bAttack = true;
	m_bSound = true;

}

CPhantom_Laser* CPhantom_Laser::Create(CGameObject* pGameObject)
{
	CPhantom_Laser* pInstance = new CPhantom_Laser();

	if (FAILED(pInstance->Initialize(pGameObject)))
	{
		MSG_BOX("Failed to Cloned : CPhantom_Laser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom_Laser::Free()
{
	__super::Free();

}

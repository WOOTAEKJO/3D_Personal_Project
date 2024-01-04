#include "stdafx.h"
#include "..\Public\PlayerState\Player_State.h"
#include "StateMachine.h"

#include "Player_Weapon_Spear.h"

CPlayer_State::CPlayer_State()
{
}

HRESULT CPlayer_State::Initialize(CGameObject* pGameObject)
{
	m_pOwner = dynamic_cast<CPlayer*>(pGameObject);
	if (m_pOwner == nullptr)
		return E_FAIL;

	m_pOwnerModel = m_pOwner->Get_BodyModel();
	Safe_AddRef(m_pOwnerModel);
	m_pOnwerTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pOnwerTransform);
	m_pOnwerNavigation = m_pOwner->Get_Component<CNavigation>();
	Safe_AddRef(m_pOnwerNavigation);
	m_pOnwerRigidBody = m_pOwner->Get_Component<CRigidBody>();
	Safe_AddRef(m_pOnwerRigidBody);
	m_pOnwerController = m_pOwner->Get_Component<CController>();
	Safe_AddRef(m_pOnwerController);

	return S_OK;
}

void CPlayer_State::State_Enter()
{
}

_uint CPlayer_State::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CPlayer_State::State_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

_uint CPlayer_State::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CPlayer_State::State_Exit()
{
}

void CPlayer_State::Trans_Attack(_bool bCheck)
{
	m_pOwner->Get_WeaponCollider()->Set_UseCol(bCheck);
}

_bool CPlayer_State::Key_Input(_float fTimeDelta)
{
	if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_FRONT)) {

		_vector vLook = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_LOOK);

		if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_RIGHT))
		{
			m_pOnwerTransform->Turn_Dir_Yaxis(m_pOnwerTransform->Get_Dir_Angle(vLook,XMVectorSet(0.f,1.f,0.f,0.f),
				XMConvertToRadians(45.f)),
				fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
		else if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_LEFT))
		{
			m_pOnwerTransform->Turn_Dir_Yaxis(m_pOnwerTransform->Get_Dir_Angle(vLook, XMVectorSet(0.f, 1.f, 0.f, 0.f),
				XMConvertToRadians(-45.f)),
				fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
		else {
			m_pOnwerTransform->Turn_Dir_Yaxis(vLook, fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
	}
	else if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_BACK))
	{
		_vector vLook = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_LOOK) * -1.f;

		if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_RIGHT))
		{
			m_pOnwerTransform->Turn_Dir_Yaxis(m_pOnwerTransform->Get_Dir_Angle(vLook, XMVectorSet(0.f, 1.f, 0.f, 0.f),
				XMConvertToRadians(-45.f)),
				fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
		else if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_LEFT))
		{
			m_pOnwerTransform->Turn_Dir_Yaxis(m_pOnwerTransform->Get_Dir_Angle(vLook, XMVectorSet(0.f, 1.f, 0.f, 0.f),
				XMConvertToRadians(45.f)),
				fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
		else {
			m_pOnwerTransform->Turn_Dir_Yaxis(vLook, fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}

	}
	else if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_RIGHT))
	{
		_vector vRight = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_RIGHT);

		if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_FRONT))
		{
			m_pOnwerTransform->Turn_Dir_Yaxis(m_pOnwerTransform->Get_Dir_Angle(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f),
				XMConvertToRadians(45.f)),
				fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
		else if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_BACK))
		{
			m_pOnwerTransform->Turn_Dir_Yaxis(m_pOnwerTransform->Get_Dir_Angle(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f),
				XMConvertToRadians(-45.f)),
				fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
		else {
			m_pOnwerTransform->Turn_Dir_Yaxis(vRight, fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}

	}
	else if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_LEFT))
	{
		_vector vRight = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_RIGHT) * -1.f;

		if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_FRONT))
		{
			m_pOnwerTransform->Turn_Dir_Yaxis(m_pOnwerTransform->Get_Dir_Angle(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f),
				XMConvertToRadians(45.f)),
				fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
		else if (m_pOnwerController->Key_Pressing(CPlayer::KEY_STATE::KEY_BACK))
		{
			m_pOnwerTransform->Turn_Dir_Yaxis(m_pOnwerTransform->Get_Dir_Angle(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f),
				XMConvertToRadians(-45.f)),
				fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
		else {
			m_pOnwerTransform->Turn_Dir_Yaxis(vRight, fTimeDelta * m_pOwner->Open_Physics_Desc()->fTurnSpeed);
		}
	}
	else
		return false;

	return true;
}

void CPlayer_State::Free()
{
	__super::Free();

}

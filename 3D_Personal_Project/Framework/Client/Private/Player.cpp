#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"

#include "Player_IDLE.h"
#include "Player_Run.h"
#include "Player_Spear_AirAttack.h"
#include "Player_Jump.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CCharacter(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	m_strModelTag = ANIMMODEL_JACK_TAG;

	CGameObject::GAMEOBJECT_DESC GameObject_Desc = {};

	GameObject_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	GameObject_Desc.fSpeedPerSec = 5.f;

	if (FAILED(CCharacter::Initialize(&GameObject_Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(1.f, 0.f, 1.f, 1.f));

	return S_OK;
}

void CPlayer::Priority_Tick(_float fTimeDelta)
{
	CCharacter::Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(_float fTimeDelta)
{
	

	//m_pModelCom->Play_Animation(fTimeDelta, true);

	CCharacter::Tick(fTimeDelta);
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	CCharacter::Late_Tick(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(CCharacter::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Bind_ShaderResources()
{
	if (FAILED(CCharacter::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Component()
{
	if (FAILED(CCharacter::Ready_Component()))
		return E_FAIL;

	/* For.Com_StateMachine*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, COM_STATEMACHINE_TAG,
		TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
		return E_FAIL;

	/* For.Com_RigidBody*/

	CRigidBody::RIGIDBODY_DESC RigidBody_Desc = {};
	RigidBody_Desc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, COM_RIGIDBODY_TAG,
		TEXT("Com_RigidBody"), reinterpret_cast<CComponent**>(&m_pRigidBodyCom),&RigidBody_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_State()
{
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CPlayer_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::RUN, CPlayer_Run::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ATTACK, CPlayer_Spear_AirAttack::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::JUMP, CPlayer_Jump::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pStateMachineCom);
	Safe_Release(m_pRigidBodyCom);

}

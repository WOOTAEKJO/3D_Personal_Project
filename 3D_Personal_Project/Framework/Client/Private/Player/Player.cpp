#include "stdafx.h"
#include "Player.h"

#include "GameInstance.h"

#include "Player_Body.h"
#include "Player_Weapon_Spear.h"

#include "Player_IDLE.h"
#include "Player_Run.h"
#include "Player_Spear_AirAttack.h"
#include "Player_Spear_Attack1.h"
#include "Player_Spear_Attack2.h"
#include "Player_Spear_Attack3.h"
#include "Player_Jump.h"
#include "Player_Roll.h"

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

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Controller()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(1.f, 0.f, 1.f, 1.f));

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_PLAYER, m_pColliderCom))) return E_FAIL;

	return S_OK;
}

void CPlayer::Priority_Tick(_float fTimeDelta)
{

	for (auto& iter : m_mapParts)
	{
		iter.second->Priority_Tick(fTimeDelta);
	}

	

	CCharacter::Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(_float fTimeDelta)
{
	
	for (auto& iter : m_mapParts)
	{
		iter.second->Tick(fTimeDelta);
	}

	CCharacter::Tick(fTimeDelta);
}

void CPlayer::Late_Tick(_float fTimeDelta)
{

	for (auto& iter : m_mapParts)
	{
		iter.second->Late_Tick(fTimeDelta);
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	CCharacter::Late_Tick(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

#ifdef _DEBUG
	m_pNavigationCom->Render();
	m_pColliderCom->Render();
#endif

	return S_OK;
}

CGameObject* CPlayer::Find_Parts(const wstring& strPartsTag)
{
	auto iter = m_mapParts.find(strPartsTag);

	if (iter == m_mapParts.end())
		return nullptr;

	return iter->second;
}

CModel* CPlayer::Get_BodyModel()
{
	CPlayer_Body* pBody = dynamic_cast<CPlayer_Body*>( Find_Parts(TEXT("Parts_Body")));
	if (pBody == nullptr)
		return nullptr;

	return pBody->Get_Component<CModel>();
}

CCollider* CPlayer::Get_WeaponCollider()
{
	CGameObject* pWeapon = Find_Parts(TEXT("Parts_Spear"));
	if (pWeapon == nullptr)
		return nullptr;

	return pWeapon->Get_Component<CCollider>();
}

void CPlayer::Mouse_Input(_float fTimeDelta)
{

}

void CPlayer::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
}

void CPlayer::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void CPlayer::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CPlayer::Bind_ShaderResources()
{
	/*if (FAILED(CCharacter::Bind_ShaderResources()))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CPlayer::Ready_Component()
{
	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = 0;
	if (FAILED(Add_Component<CNavigation>(COM_NAVIGATION_TAG, &m_pNavigationCom, &NavigationDesc))) return E_FAIL;

	if (FAILED(Add_Component<CStateMachine>(COM_STATEMACHINE_TAG, &m_pStateMachineCom))) return E_FAIL;

	CRigidBody::RIGIDBODY_DESC RigidBody_Desc = {};
	RigidBody_Desc.pOwner = this;
	if (FAILED(Add_Component<CRigidBody>(COM_RIGIDBODY_TAG, &m_pRigidBodyCom,&RigidBody_Desc))) return E_FAIL;

	/*CBounding_AABB::BOUNDING_AABB_DESC AABB_Desc = {};
	AABB_Desc.eType = CBounding::TYPE::TYPE_AABB;
	AABB_Desc.vExtents = _float3(0.5f, 1.f, 0.5f);
	AABB_Desc.vCenter = _float3(0.f, AABB_Desc.vExtents.y, 0.f);*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	//Sphere_Desc.bUseCol = true;
	Sphere_Desc.fRadius = 0.9f;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom,&Sphere_Desc))) return E_FAIL;

	if (FAILED(Add_Component<CController>(COM_CONTROLLER_TAG, &m_pControllerCom))) return E_FAIL;
	
	return S_OK;
}

HRESULT CPlayer::Ready_State()
{
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CPlayer_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::RUN, CPlayer_Run::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ATTACK1, CPlayer_Spear_Attack1::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ATTACK2, CPlayer_Spear_Attack2::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ATTACK3, CPlayer_Spear_Attack3::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::AIR_ATTACK, CPlayer_Spear_AirAttack::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::JUMP, CPlayer_Jump::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ROLL, CPlayer_Roll::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Parts()
{
	CPlayer_Body::PLAYERBODY_DESC PlayerBody_Desc = {};
	PlayerBody_Desc.pParentsTransform = m_pTransformCom;
	if (FAILED(Add_Parts(GO_PLAYER_BODY_TAG, TEXT("Parts_Body"),&PlayerBody_Desc))) return E_FAIL;

	CPlayer_Weapon_Spear::PLAYERSPEAR_DESC PlayerSpear_Desc = {};
	PlayerSpear_Desc.pParentsTransform = m_pTransformCom;
	PlayerSpear_Desc.pBones = Get_BodyModel()->Get_Bones();
	if (FAILED(Add_Parts(GO_PLAYER_SPEAR_TAG, TEXT("Parts_Spear"), &PlayerSpear_Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Controller()
{
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_FRONT,DIK_W))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_BACK, DIK_S))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_RIGHT, DIK_D))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_LEFT, DIK_A))) return E_FAIL;

	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_JUMP, DIK_SPACE))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_ROLL, DIK_LSHIFT))) return E_FAIL;

	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_LB_ATTACK, DIM_LB))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_RB_ATTACK, DIM_RB))) return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Add_Parts(const wstring& strPrototypeTag, const wstring& strPartsTag, void* pArg)
{
	CGameObject* pParts = nullptr;

	pParts = m_pGameInstance->Add_Independent_Clone(strPrototypeTag, pArg);
	if (pParts == nullptr)
		return E_FAIL;

	if (Find_Parts(strPartsTag) != nullptr)
		return E_FAIL;

	m_mapParts.emplace(strPartsTag, pParts);

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

	for (auto& iter : m_mapParts)
		Safe_Release(iter.second);
	m_mapParts.clear();

	Safe_Release(m_pControllerCom);
}

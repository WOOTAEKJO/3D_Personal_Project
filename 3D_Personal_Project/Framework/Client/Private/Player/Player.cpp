#include "stdafx.h"
#include "Player.h"

#include "GameInstance.h"

#include "Player_Body.h"
#include "Player_Weapon_Spear.h"
#include "Player_Weapon_Shovel.h"
#include "Crow.h"

#include "Player_IDLE.h"
#include "Player_Run.h"
#include "Player_Spear_AirAttack.h"
#include "Player_Spear_Attack1.h"
#include "Player_Spear_Attack2.h"
#include "Player_Spear_Attack3.h"
#include "Player_Jump.h"
#include "Player_Roll.h"
#include "Player_Land.h"
#include "Player_Fall.h"

#include "Range_Bullet.h"

#include "Particle.h"

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
	CHARACTER_DESC Character_Desc = {};

	Character_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Character_Desc.fSpeedPerSec = 5.f;
	Character_Desc.strModelTag = ANIMMODEL_JACK_TAG;

	if (FAILED(CCharacter::Initialize(&Character_Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Controller()))
		return E_FAIL;

	if (FAILED(Init_Point_Light()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.16f, 0.16f, 0.16f);

	if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_GAMEPLAY)
	{
		//m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(4.f, 7.f, 4.f, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(39.53f, 9.f, 28.438f, 1.f));
		m_pNavigationCom->Set_CurrentIndex(397);
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("OwlTalk"), TEXT("Player"), this))) return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("OwlTalk2"), TEXT("Player"), this))) return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("OwlTalk3"), TEXT("Player"), this))) return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("CrowTalk"), TEXT("Player"), this))) return E_FAIL;

	}
	else if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_BOSS1)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(10.f, 2.f, 13.f, 1.f));
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("Boss1Talk"), TEXT("Player"), this))) return E_FAIL;
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("Boss2Talk"), TEXT("Player"), this))) return E_FAIL;
		
	}		
	else if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_BOSS2)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(16.5f, 7.f, 18.5f, 1.f));
	}
		

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
	//ShadowLight_SetUp();

	for (auto& iter : m_mapParts)
	{
		iter.second->Tick(fTimeDelta);
	}

	CCharacter::Tick(fTimeDelta);
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	NextAttackID();

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	for (auto& iter : m_mapParts)
	{
		iter.second->Late_Tick(fTimeDelta);
	}

	if (FAILED(m_pGameInstance->Add_DebugRender(m_pNavigationCom))) return;
	if (FAILED(m_pGameInstance->Add_DebugRender(m_pColliderCom))) return;

	Update_Light();

	CCharacter::Late_Tick(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	

	return S_OK;
}

CGameObject* CPlayer::Find_Parts(PARTS_TYPE ePartsTag)
{
	auto iter = m_mapParts.find(ePartsTag);

	if (iter == m_mapParts.end())
		return nullptr;

	return iter->second;
}

CModel* CPlayer::Get_BodyModel()
{
	CPlayer_Body* pBody = dynamic_cast<CPlayer_Body*>( Find_Parts(PARTS_TYPE::PARTS_BODY));
	if (pBody == nullptr)
		return nullptr;

	return pBody->Get_Component<CModel>();
}

CTransform* CPlayer::Get_BodyTransform()
{
	CPlayer_Body* pBody = dynamic_cast<CPlayer_Body*>(Find_Parts(PARTS_TYPE::PARTS_BODY));
	if (pBody == nullptr)
		return nullptr;

	return pBody->Get_Component<CTransform>();
}

CCollider* CPlayer::Get_WeaponCollider()
{
	CGameObject* pWeapon = Find_Parts(PARTS_TYPE::PARTS_WEAPON);
	if (pWeapon == nullptr)
		return nullptr;

	return pWeapon->Get_Component<CCollider>();
}

_int CPlayer::Get_CurrentState()
{
	if (m_pStateMachineCom == nullptr)
		return -1;

	return m_pStateMachineCom->Get_StateID();
}

void CPlayer::Animation_By_Type(STATE eType)
{
	_int iAnimIndex = Find_AnimIndex(m_eCurrentWeaponType, eType);
	if (iAnimIndex == -1)
		return;

	CPlayer_Body* pBody = dynamic_cast<CPlayer_Body*>(Find_Parts(PARTS_TYPE::PARTS_BODY));
	if (pBody == nullptr)
		return;

	pBody->Set_AnimationIndex(iAnimIndex);
}

void CPlayer::Create_Range_Bullet()
{
	CRange_Bullet::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_PLAYER_BULLET;
	BulletDesc.fRadius = 0.25f;
	BulletDesc.fLifeTime = 0.03f;
	BulletDesc.fSpeed = 0.f;
	BulletDesc.pTarget = nullptr;

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS) +
		XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK)) * 0.3f;

	vPos.m128_f32[1] -= BulletDesc.fRadius;

	XMStoreFloat4(&BulletDesc.fStartPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_RANGE_BULLET_TAG, &BulletDesc)))
		return;
}

void CPlayer::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	if (iColID == m_pColliderCom->Get_Collider_ID())
	{
		if ((pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_MONSTER_BULLET || 
			pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_TRAP)
			&& !m_Status_Desc.bHited)
		{
			m_bHit_Effect = true;
			//m_Status_Desc.bHited = true;
			if(m_Status_Desc.iCurHP > 1)
				m_Status_Desc.iCurHP -= 1;

			CParticle::PARTICLEINFO Info = {};
			Info.pOwner = this;
			Info.strParticleTag = PARTICLE_JACKHITTAG;
			Info.fLifeTime = 1.f;
			Info.pBones = Get_BodyModel()->Get_Bones();

			if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
				GO_PARTICLENORMAL_TAG, &Info)))
				return;

			Create_Damage_Effect(0.3f,TEX_DAMAGEIMPACT_TAG);
		}
	}
}

void CPlayer::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
	if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_MONSTER)
	{
		Pushed();
	}
}

void CPlayer::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
	Pushed_Reset();
}

HRESULT CPlayer::Bind_ShaderResources()
{
	if (FAILED(m_mapParts[PARTS_TYPE::PARTS_BODY]->Get_Component<CShader>()->Bind_RawValue("g_bHited", &m_bHit_Effect,
		sizeof(_bool)))) return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Ready_Component()
{
	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = 0;
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom, &NavigationDesc))) return E_FAIL;

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
	Sphere_Desc.bUseCol = true;
	Sphere_Desc.fRadius = 0.9f;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom,&Sphere_Desc))) return E_FAIL;

	if (FAILED(Add_Component<CController>(COM_CONTROLLER_TAG, &m_pControllerCom))) return E_FAIL;
	
	return S_OK;
}

HRESULT CPlayer::Init_Point_Light()
{
	LIGHT_DESC LightDesc = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] = m_pTransformCom->Get_Scaled().y;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 0.4f;
	LightDesc.vDiffuse = _float4(1.f, 0.6f, 0.4f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 0.6f, 0.4f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

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
	if (FAILED(m_pStateMachineCom->Add_State(STATE::LAND, CPlayer_Land::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ROLL, CPlayer_Roll::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::FALL, CPlayer_Fall::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Parts()
{
	CPlayer_Body::PLAYERBODY_DESC PlayerBody_Desc = {};
	PlayerBody_Desc.pParentsTransform = m_pTransformCom;
	if (FAILED(Add_Parts(GO_PLAYER_BODY_TAG, PARTS_TYPE::PARTS_BODY,&PlayerBody_Desc))) return E_FAIL;

	/*CPlayer_Weapon_Spear::PLAYERSPEAR_DESC PlayerSpear_Desc = {};
	PlayerSpear_Desc.pParentsTransform = m_pTransformCom;
	PlayerSpear_Desc.pBones = Get_BodyModel()->Get_Bones();
	if (FAILED(Add_Parts(GO_PLAYER_SPEAR_TAG, TEXT("Parts_Weapon"), &PlayerSpear_Desc))) return E_FAIL;*/

	CPlayer_Weapon_Shovel::PLAYERSHOVEL_DESC PlayerShovel_Desc = {};
	PlayerShovel_Desc.pParentsTransform = m_pTransformCom;
	PlayerShovel_Desc.pBones = Get_BodyModel()->Get_Bones();
	PlayerShovel_Desc.pOwner = this;
	if (FAILED(Add_Parts(GO_PLAYER_SHOVEL_TAG, PARTS_TYPE::PARTS_WEAPON, &PlayerShovel_Desc))) return E_FAIL;

	/*CCrow::CROW_DESC Crow_Desc = {};
	Crow_Desc.pPlayer = this;
	Crow_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Crow_Desc.fSpeedPerSec = 5.f;
	Crow_Desc.strModelTag = ANIMMODEL_CROW_TAG;
	if (FAILED(Add_Parts(GO_CROW_TAG, PARTS_TYPE::PARTS_CROW, &Crow_Desc))) return E_FAIL;*/

	return S_OK;
}

HRESULT CPlayer::Ready_Controller()
{
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_FRONT,DIK_W))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_BACK, DIK_S))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_RIGHT, DIK_D))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_LEFT, DIK_A))) return E_FAIL;

	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_JUMP, DIK_SPACE))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_ROLL, DIK_E))) return E_FAIL;

	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_LB_ATTACK, DIM_LB))) return E_FAIL;
	if (FAILED(m_pControllerCom->Add_ControllKey(KEY_STATE::KEY_RB_ATTACK, DIM_RB))) return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Animation()
{
	WEAPON_TYPE Weapon_Type = WEAPON_TYPE::TYPE_SPEAR;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::IDLE, 83))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::RUN, 86))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::ATTACK1, 79))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::ATTACK2, 80))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::ATTACK3, 81))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::JUMP, 84))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::ROLL, 114))) return E_FAIL;

	Weapon_Type = WEAPON_TYPE::TYPE_SHOVEL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::IDLE, 74))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::RUN, 55))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::ATTACK1, 118))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::ATTACK2, 119))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::ATTACK3, 120))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::AIR_ATTACK, 19))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::JUMP, 107))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::DOUBLEJUMP, 102))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::FALL, 104))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::LAND, 108))) return E_FAIL;
	if(FAILED(Add_WeaponType_By_Animation(Weapon_Type, CPlayer::ROLL, 114))) return E_FAIL;

	m_eCurrentWeaponType = WEAPON_TYPE::TYPE_SHOVEL;

	return S_OK;
}

HRESULT CPlayer::Ready_Act()
{

	return S_OK;
}

HRESULT CPlayer::Add_Parts(const wstring& strPrototypeTag, PARTS_TYPE ePartsTag, void* pArg)
{
	CGameObject* pParts = nullptr;

	pParts = m_pGameInstance->Add_Independent_Clone(strPrototypeTag, pArg);
	if (pParts == nullptr)
		return E_FAIL;

	if (Find_Parts(ePartsTag) != nullptr)
		return E_FAIL;

	m_mapParts.emplace(ePartsTag, pParts);

	return S_OK;
}

HRESULT CPlayer::Add_WeaponType_By_Animation(WEAPON_TYPE eWeaponType, STATE eStateType, _uint iAnimIndex)
{
	if (eWeaponType >= WEAPON_TYPE::TYPE_END || eStateType >= STATE::STATE_END)
		return E_FAIL;

	auto& WeaponType = m_mapTypeAnimation.find(eWeaponType);
	if (WeaponType == m_mapTypeAnimation.end())
	{
		ANIMINDEX mapAnimIndex;
		mapAnimIndex.emplace(eStateType, iAnimIndex);
		m_mapTypeAnimation.emplace(eWeaponType, mapAnimIndex);
	}
	else {
		WeaponType->second.emplace(eStateType, iAnimIndex);
	}

	return S_OK;
}

_int CPlayer::Find_AnimIndex(WEAPON_TYPE eWeaponType, STATE eStateType)
{
	auto& WeaponType = m_mapTypeAnimation.find(eWeaponType);
	if (WeaponType == m_mapTypeAnimation.end())
		return -1;

	auto& StateType = WeaponType->second.find(eStateType);
	if (StateType == WeaponType->second.end())
		return -1;

	return StateType->second;
}

void CPlayer::NextAttackID()
{
	switch (m_pStateMachineCom->Get_PrevID())
	{
	case (_uint)CPlayer::STATE::ATTACK1:
		m_iAttackID = (_uint)CPlayer::STATE::ATTACK2;
		break;
	case (_uint)CPlayer::STATE::ATTACK2:
		m_iAttackID = (_uint)CPlayer::STATE::ATTACK3;
		break;
	case (_uint)CPlayer::STATE::ATTACK3:
		m_iAttackID = (_uint)CPlayer::STATE::ATTACK1;
		break;
	default:
		break;
	}
}

void CPlayer::ShadowLight_SetUp()
{
	SHADOW_LIGHT_DESC Shadow_Desc = {};

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	//XMStoreFloat4(&Shadow_Desc.vPos, XMVectorAdd(vPos, XMVectorSet(-5.f, 5.f, -5.f, 0.f)));
	XMStoreFloat4(&Shadow_Desc.vAt, vPos);
	Shadow_Desc.vPos = _float4(40.f, 40.f, 40.f, 1.f);
	Shadow_Desc.vUpDir = _float4(0.f, 1.f, 0.f, 0.f);

	Shadow_Desc.fFov = XMConvertToRadians(60.f);
	Shadow_Desc.fAspect =((_float)g_iWinSizeX / g_iWinSizeY);
	Shadow_Desc.fNear = 0.1f;
	Shadow_Desc.fFar = 600.f;

	//m_pGameInstance->Get_ShadowLight()->Open_Light_Desc();
	m_pGameInstance->Get_ShadowLight()->Set_Light_Desc(Shadow_Desc);
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

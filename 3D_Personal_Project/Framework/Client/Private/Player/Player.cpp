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

#include "Utility_Effect.h"

#include "UI.h"
#include "UI_HP.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter(pDevice, pContext)
{// 디바이스와 디바이스 컨텍스트를 받아 초기화
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CCharacter(rhs),m_pMaxHP(rhs.m_pMaxHP),m_pCurHP(rhs.m_pCurHP)
{// 복사 생성자
}

HRESULT CPlayer::Initialize_Prototype()
{// 원형 초기화
	m_pMaxHP = new _int;
	m_pCurHP = new _int;

	*m_pMaxHP = 100;
	*m_pCurHP = *m_pMaxHP;

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{// 사본 초기화
	CHARACTER_DESC Character_Desc = {};

	Character_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Character_Desc.fSpeedPerSec = 5.f;
	Character_Desc.strModelTag = ANIMMODEL_JACK_TAG;
	// 플레이어 기본 상태 초기화

	if (FAILED(CCharacter::Initialize(&Character_Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL; // 컴포넌트 준비

	if (FAILED(Ready_Parts()))
		return E_FAIL; // 파츠 준비

	if (FAILED(Ready_Animation()))
		return E_FAIL; // 애니메이션 준비

	if (FAILED(Ready_State()))
		return E_FAIL; // 상태 준비

	if (FAILED(Ready_Controller()))
		return E_FAIL; // 컨트롤러 준비

	if (FAILED(Init_Point_Light()))
		return E_FAIL; // 포인트 라이트 초기화

	if (FAILED(Ready_UI()))
		return E_FAIL; // UI 준비

	m_pTransformCom->Set_Scaling(0.16f, 0.16f, 0.16f);

	if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_GAMEPLAY)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(4.f, 7.f, 4.f, 1.f));
		
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
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("Ending2"), TEXT("Player"), this))) return E_FAIL;
	}
		
	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_PLAYER, m_pColliderCom))) return E_FAIL;
	// 콜라이더 설정

	_vector vTmp = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vTmp.m128_f32[1] += m_pTransformCom->Get_Scaled().y;

	_float4 vEffectPos;
	XMStoreFloat4(&vEffectPos, vTmp);

	CBone* pBone = Get_BodyModel()->Get_Bones()[7];

	CUtility_Effect::Create_Effect_Light(m_pGameInstance, this, pBone,
		MASK_GLOWTEST_TAG, _float2(25000.f, 25000.f),
		vEffectPos, _float4(1.f, 0.6f, 0.4f, 1.f), 0.3f,&m_pLightEffect);
	// 라이트 이펙트 생성

	m_Status_Desc.iMaxHP = *m_pMaxHP;
	m_Status_Desc.iCurHP = m_Status_Desc.iMaxHP;

	return S_OK;
}

void CPlayer::Priority_Tick(_float fTimeDelta)
{// 우선순위 틱
	m_Status_Desc.iCurHP = *m_pCurHP;

	for (auto& iter : m_mapParts)
	{
		iter.second->Priority_Tick(fTimeDelta);
	}

	CCharacter::Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(_float fTimeDelta)
{// 일반적인 틱
	for (auto& iter : m_mapParts)
	{
		iter.second->Tick(fTimeDelta);
	}

	CCharacter::Tick(fTimeDelta);
}

void CPlayer::Late_Tick(_float fTimeDelta)
{// 늦은 틱
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
		return E_FAIL; // 셰이더 리소스 바인딩

	return S_OK;
}

CGameObject* CPlayer::Find_Parts(PARTS_TYPE ePartsTag)
{// 특정 파츠 검색후 반환
	auto iter = m_mapParts.find(ePartsTag);

	if (iter == m_mapParts.end())
		return nullptr;

	return iter->second;
}

CModel* CPlayer::Get_BodyModel()
{// 몸체 모델 컴포넌트 반환
	CPlayer_Body* pBody = dynamic_cast<CPlayer_Body*>( Find_Parts(PARTS_TYPE::PARTS_BODY));
	if (pBody == nullptr)
		return nullptr;

	return pBody->Get_Component<CModel>();
}

CTransform* CPlayer::Get_BodyTransform()
{// 몸체 트렌스폼 컴포넌트 반환
	CPlayer_Body* pBody = dynamic_cast<CPlayer_Body*>(Find_Parts(PARTS_TYPE::PARTS_BODY));
	if (pBody == nullptr)
		return nullptr;

	return pBody->Get_Component<CTransform>();
}

CCollider* CPlayer::Get_WeaponCollider()
{// 무기 콜라이더 컴포넌트 반환
	CGameObject* pWeapon = Find_Parts(PARTS_TYPE::PARTS_WEAPON);
	if (pWeapon == nullptr)
		return nullptr;

	return pWeapon->Get_Component<CCollider>();
}

_int CPlayer::Get_CurrentState()
{// 현재 상태 반환
	if (m_pStateMachineCom == nullptr)
		return -1;

	return m_pStateMachineCom->Get_StateID();
}

void CPlayer::Animation_By_Type(STATE eType)
{// 상태에 맞는 애니메이션으로 전환
	_int iAnimIndex = Find_AnimIndex(m_eCurrentWeaponType, eType);
	if (iAnimIndex == -1)
		return;

	CPlayer_Body* pBody = dynamic_cast<CPlayer_Body*>(Find_Parts(PARTS_TYPE::PARTS_BODY));
	if (pBody == nullptr)
		return;

	pBody->Set_AnimationIndex(iAnimIndex);
}

void CPlayer::Create_Range_Bullet()
{// 특정 총알 생성
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
{// 충돌 발생 시
	if (iColID == m_pColliderCom->Get_Collider_ID())
	{
		if ((pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_MONSTER_BULLET || 
			pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_TRAP)
			&& !m_Status_Desc.bHited)
		{
			m_bHit_Effect = true;

			/*if(m_Status_Desc.iCurHP > 10)
				m_Status_Desc.iCurHP -= 1;*/
			if (*m_pCurHP > 10)
				*m_pCurHP -= 1;

			CParticle::PARTICLEINFO Info = {};
			Info.pOwner = this;
			Info.strParticleTag = PARTICLE_JACKHITTAG;
			Info.fLifeTime = 1.f;
			Info.pBones = Get_BodyModel()->Get_Bones();

			if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
				GO_PARTICLENORMAL_TAG, &Info)))
				return;

			Create_Damage_Effect(0.3f,TEX_DAMAGEIMPACT_TAG);
			m_pGameInstance->Play_Sound(L"Jack", L"HitVoice.ogg", CHANNELID::SOUND_PLAYER_VOICE, 1.f);
			m_pGameInstance->Play_Sound(L"Jack", L"Hit.ogg", CHANNELID::SOUND_PLAYER_HIT, 0.3f);
		}
	}
}

void CPlayer::OnCollisionStay(CCollider* pCollider, _uint iColID)
{// 충돌 유지 시
	if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_MONSTER)
	{
		Pushed();
	}
}

void CPlayer::OnCollisionExit(CCollider* pCollider, _uint iColID)
{// 충돌 종료 시
	Pushed_Reset();
}

HRESULT CPlayer::Bind_ShaderResources()
{// 셰이더 리소스 바인딩
	if (FAILED(m_mapParts[PARTS_TYPE::PARTS_BODY]->Get_Component<CShader>()->Bind_RawValue("g_bHited", &m_bHit_Effect,
		sizeof(_bool)))) return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Ready_Component()
{// 컴포넌트 준비
	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = 0;
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom, &NavigationDesc))) return E_FAIL;

	if (FAILED(Add_Component<CStateMachine>(COM_STATEMACHINE_TAG, &m_pStateMachineCom))) return E_FAIL;

	CRigidBody::RIGIDBODY_DESC RigidBody_Desc = {};
	RigidBody_Desc.pOwner = this;
	if (FAILED(Add_Component<CRigidBody>(COM_RIGIDBODY_TAG, &m_pRigidBodyCom,&RigidBody_Desc))) return E_FAIL;

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
{// 포인트 라이트 초기화
	LIGHT_DESC LightDesc = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] = m_pTransformCom->Get_Scaled().y;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 0.4f;
	LightDesc.vDiffuse = _float4(1.f, 0.6f, 0.4f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

	return S_OK;
}

HRESULT CPlayer::Ready_UI()
{// UI 준비

	CUI_HP::UI_HP_DESC Hp_Desc = {};

	Hp_Desc.strTextureTag = TEX_WATER_TAG;
	Hp_Desc.strMaskTexture = UI_LIFEBARMASK_TAG;
	Hp_Desc.strNoiseTexture = UI_SUBBAR_TAG;
	Hp_Desc.vSolidColor = _float4(0.f, 0.2f, 0.f, 1.f);
	Hp_Desc.vCenterPos = _float2(g_iWinSizeX * 0.15f, (_float)g_iWinSizeY - g_iWinSizeY * 0.1f);
	Hp_Desc.vScale = _float2(350.f, 80.f);
	Hp_Desc.pOwner = this;
	Hp_Desc.iShaderPassIndx = 10;
	Hp_Desc.bRender = true;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_UI]
		, GO_UIHP_TAG, &Hp_Desc))) return E_FAIL;

	CUI::UI_DESC Desc = {};

	Desc.strTextureTag = UI_CROWLIFEBAR_TAG;
	Desc.vCenterPos = _float2(Hp_Desc.vCenterPos.x + Hp_Desc.vScale.x * 0.01f, Hp_Desc.vCenterPos.y - Hp_Desc.vScale.y * 0.51f);
	Desc.vScale = _float2(360.f, 100.f);
	Desc.bRender = true;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_UI]
		, GO_UICHATBOX_TAG, &Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_State()
{// 상태 준비
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
{// 파츠 준비
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

	return S_OK;
}

HRESULT CPlayer::Ready_Controller()
{// 컨트롤러 준비
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
{// 무기 타입에 따른 애니메이션 준비
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
{// 파츠 추가
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
{// 무기 타입에 따른 애니메이션 추가
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
{// 특정 무기 타입에 따른 애니메이션 검색
	auto& WeaponType = m_mapTypeAnimation.find(eWeaponType);
	if (WeaponType == m_mapTypeAnimation.end())
		return -1;

	auto& StateType = WeaponType->second.find(eStateType);
	if (StateType == WeaponType->second.end())
		return -1;

	return StateType->second;
}

void CPlayer::NextAttackID()
{// 다음 연계 공격을 준비
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

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{// 원형 객체 생성
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{// 사본 객체 생성
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{// 메모리 해제
	__super::Free();

	if (m_isCloned == false)
	{
		Safe_Delete(m_pMaxHP);
		Safe_Delete(m_pCurHP);
	}

	for (auto& iter : m_mapParts)
		Safe_Release(iter.second);
	m_mapParts.clear();

	Safe_Release(m_pControllerCom);
}

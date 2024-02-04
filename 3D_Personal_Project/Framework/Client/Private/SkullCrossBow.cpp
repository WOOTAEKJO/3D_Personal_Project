#include "stdafx.h"
#include "..\Public\SkullCrossBow.h"

#include "NorMonster_IDLE.h"
#include "NorMonster_Move.h"
#include "NorMonster_Attack.h"
#include "NorMonster_Delay.h"
#include "NorMonster_Dead.h"
#include "NorMonster_Hited.h"

#include "Bone.h"

#include "Normal_Bullet.h"

#include "Light.h"

#include "Effect_Energy.h"

CSkullCrossBow::CSkullCrossBow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CSkullCrossBow::CSkullCrossBow(const CSkullCrossBow& rhs)
	:CMonster(rhs)
{
}

HRESULT CSkullCrossBow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkullCrossBow::Initialize(void* pArg)
{

	if (FAILED(CMonster::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_eMonsterType = CMonster::MONSTER_TYPE::SKULLCROSSBOW;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_MONSTER, m_pColliderCom)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_MONSTER_BULLET, m_pWeaponColliderCom)))
		return E_FAIL;*/

	/*m_pSocketBone = m_pModelCom->Get_Bone(34);
	Safe_AddRef(m_pSocketBone);*/

	if (FAILED(m_pGameInstance->Load_Data_Json(m_strModelTag, this)))
		return E_FAIL;

	m_Status_Desc.iMaxHP = 5;
	m_Status_Desc.iCurHP = 5;

	m_Status_Desc.fDetection_Range = 5.f;
	m_Status_Desc.fAttack_Range = 3.f;

	m_Physics_Desc.fForwardSpeed = 0.3f;

	m_bActivate = false;

	if (FAILED(Init_Point_Light()))
		return E_FAIL;


	return S_OK;
}

void CSkullCrossBow::Priority_Tick(_float fTimeDelta)
{
	Monster_Dead();

	CMonster::Priority_Tick(fTimeDelta);
}

void CSkullCrossBow::Tick(_float fTimeDelta)
{
	CMonster::Tick(fTimeDelta);
}

void CSkullCrossBow::Late_Tick(_float fTimeDelta)
{

	CMonster::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_SHADOW, this)))
		return;
}

HRESULT CSkullCrossBow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(CMonster::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkullCrossBow::Render_Shadow()
{
	if (FAILED(CMonster::Render_Shadow()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkullCrossBow::Create_Bullet()
{
	CGameObject* pBullet = nullptr;

	CNormal_Bullet::BULLET_NORMAL_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.03f;
	BulletDesc.fLifeTime = 2.f;
	BulletDesc.fSpeed = 3.f;
	BulletDesc.pTarget = m_pPlayer;
	BulletDesc.vTrailColor = _float4(0.8f, 0.2f, 0.2f, 1.f);

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS) +
		XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK)) * 0.1f;

	vPos.m128_f32[1] += m_pTransformCom->Get_Scaled().y;

	XMStoreFloat4(&BulletDesc.fStartPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_NORMAL_BULLET_TAG, &BulletDesc, reinterpret_cast<CGameObject**>(&pBullet))))
		return E_FAIL;

	CEffect_Energy::EFFECT_ENERGYINFO Info = {};
	Info.pOwner = pBullet;
	Info.fLifeTime = 0.f;
	Info.strEffectTextureTag = TEX_BUBLE_TAG;
	Info.vSize = _float2(0.07f, 0.07f);
	Info.vColor = _float4(0.8f, 0.2f, 0.2f, 1.f);
	Info.fSizeSpeed = -0.01f;

	CTransform* pTrans = pBullet->Get_Component<CTransform>();

	_vector vBulletPos = pTrans->Get_State(CTransform::STATE::STATE_POS);
	vBulletPos.m128_f32[1] += pTrans->Get_Scaled().y;

	XMStoreFloat4(&Info.vPos, vBulletPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTENERGY_TAG, &Info)))
		return E_FAIL;

	return S_OK;
}

void CSkullCrossBow::Load_FromJson(const json& In_Json)
{
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Load_FromJson(In_Json);
}

void CSkullCrossBow::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	if (!m_bActivate)
		return;

	if (iColID == m_pColliderCom->Get_Collider_ID())
	{
		if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_PLAYER_BULLET 
			&&!m_bHit_Effect)
		{
			if (m_pStateMachineCom->Get_StateID() != (_uint)STATE::ATTACK)
			{
				m_Status_Desc.bHited = true;
			}
			m_bHit_Effect = true;
			m_Status_Desc.iCurHP -= 1;

			Create_Damage_Effect(0.3f, TEX_DAMAGEIMPACT_TAG);

			m_pGameInstance->Play_Sound(L"Spooketon", L"HitVoice.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.5f);
			m_pGameInstance->Play_Sound(L"Spooketon", L"Hit.ogg", CHANNELID::SOUND_MONSTER_HIT, 0.3f);
		}
	}
}

void CSkullCrossBow::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
	if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_PLAYER ||
		pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_MONSTER)
	{
		Pushed();
	}
}

void CSkullCrossBow::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
	Pushed_Reset();
}

HRESULT CSkullCrossBow::Bind_ShaderResources()
{
	if (FAILED(CMonster::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkullCrossBow::Ready_Component()
{
	if (FAILED(CMonster::Ready_Component()))
		return E_FAIL;

	/*CBounding_AABB::BOUNDING_AABB_DESC AABB_Desc = {};
	AABB_Desc.pOnwer = this;
	AABB_Desc.eType = CBounding::TYPE::TYPE_AABB;
	AABB_Desc.bUseCol = true;
	AABB_Desc.vExtents = _float3(0.5f, 1.f, 0.5f);
	AABB_Desc.vCenter = _float3(0.f, AABB_Desc.vExtents.y, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &AABB_Desc))) return E_FAIL;*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.bUseCol = true;
	Sphere_Desc.fRadius = 0.7f;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CSkullCrossBow::Ready_State()
{

	if (FAILED(__super::Ready_State()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkullCrossBow::Init_Point_Light()
{
	LIGHT_DESC LightDesc = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] = m_pTransformCom->Get_Scaled().y;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 0.3f;
	LightDesc.vDiffuse = _float4(0.8f, 0.2f, 0.2f, 1.f);
	LightDesc.vAmbient = _float4(0.8f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

	return S_OK;
}

CSkullCrossBow* CSkullCrossBow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkullCrossBow* pInstance = new CSkullCrossBow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkullCrossBow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSkullCrossBow::Clone(void* pArg)
{
	CSkullCrossBow* pInstance = new CSkullCrossBow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkullCrossBow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkullCrossBow::Free()
{
	__super::Free();
	
}

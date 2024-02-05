#include "stdafx.h"
#include "..\Public\HelicoScarrow.h"

#include "Helico_Dead.h"
#include "Helico_Dive.h"
#include "Helico_Dive_IDLE.h"
#include "Helico_Exhausted_Hit.h"
#include "Helico_Exhausted_IDLE.h"
#include "Helico_Exhausted_Wake.h"
#include "Helico_Attack.h"
#include "Helico_Attack_End.h"
#include "Helico_Attack_Start.h"
#include "Helico_IDLE.h"
#include "Helico_Dive_Up.h"
#include "Helico_Ground_Smash.h"

#include "Bone.h"

#include "Helico_Bullet.h"
#include "Shock_Wave.h"

#include "Utility_Effect.h"
#include "Effect_Reaper.h"

CHelicoScarrow::CHelicoScarrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHelicoScarrow::CHelicoScarrow(const CHelicoScarrow& rhs)
	:CMonster(rhs)
{
}

HRESULT CHelicoScarrow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHelicoScarrow::Initialize(void* pArg)
{

	if (FAILED(CMonster::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_MONSTER, m_pColliderCom)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Load_Data_Json(m_strModelTag, this)))
		return E_FAIL;*/

	if (FAILED(Init_Point_Light()))
		return E_FAIL;

	m_pSocketBone = m_pModelCom->Get_Bone(2);
	Safe_AddRef(m_pSocketBone);

	/*m_Status_Desc.iMaxHP = 5;
	m_Status_Desc.iCurHP = 5;*/

	m_Status_Desc.bAttack_able = false;
	m_Status_Desc.bTalk = true;

	if (FAILED(m_pGameInstance->Add_Actor(TEXT("Boss1Talk"), TEXT("Boss1"), this))) return E_FAIL;

	return S_OK;
}

void CHelicoScarrow::Priority_Tick(_float fTimeDelta)
{
	if (!m_bActivate)
		return;

	if (m_bActivate && (m_pLightEffect == nullptr))
	{
		_vector vTmp = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
		vTmp.m128_f32[1] += m_pTransformCom->Get_Scaled().y;

		_float4 vEffectPos;
		XMStoreFloat4(&vEffectPos, vTmp);

		CBone* pBone = m_pModelCom->Get_Bones()[16];

		CUtility_Effect::Create_Effect_Light(m_pGameInstance, this, pBone,
			MASK_GLOWTEST_TAG, _float2(10.f, 10.f),
			vEffectPos, _float4(1.f, 0.8f, 0.f, 1.f), 0.3f, &m_pLightEffect);
	}

	m_pColliderCom->Update(m_pSocketBone->Get_CombinedTransformationMatrix() * m_pTransformCom->Get_WorldMatrix_Matrix());
	CGameObject::Priority_Tick(fTimeDelta);
}

void CHelicoScarrow::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_0))
	{
		m_pStateMachineCom->Set_State(CHelicoScarrow::STATE::DEAD);
	}

	Shock_Wave_Radius_Compute();

	CMonster::Tick(fTimeDelta);
}

void CHelicoScarrow::Late_Tick(_float fTimeDelta)
{

	CMonster::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_SHADOW, this)))
		return;
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLUR, this)))
		return;
}

HRESULT CHelicoScarrow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(CMonster::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHelicoScarrow::Render_Shadow()
{
	if (FAILED(CMonster::Render_Shadow()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHelicoScarrow::Render_Blur()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iIndx[4] = { 1,2,6,7 };

	for (size_t i = 0; i < 4; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", iIndx[i])))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", iIndx[i], TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(iIndx[i]);
	}

	return S_OK;
}

_bool CHelicoScarrow::Is_AllHited(_uint iNum)
{
	if (iNum >= 2)
		return false;

	if (m_iHited_Count[iNum] >= 3)
		return true;

	return false;
}

void CHelicoScarrow::Reset_Hited()
{
	m_iHited_Count[1] = 0;
}

void CHelicoScarrow::Create_Monster()
{
	CGameObject* pMonst = nullptr;
	CCharacter::CHARACTER_DESC Character_Desc = {};

	_float4 vPos[3] = { _float4(18.131f,2.f,19.256,1.f),_float4(18.001f,2.f,20.420f,1.f),
	_float4(19.168f,2.f,20.090f,1.f) };

	for (_uint i = 0; i < 3; i++)
	{
		Character_Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Character_Desc.fSpeedPerSec = 5.f;
		Character_Desc.strModelTag = ANIMMODEL_SPOOKETON_TAG;

		if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_MONSTER],
			ANIMMODEL_SPOOKETON_TAG, &Character_Desc, reinterpret_cast<CGameObject**>(&pMonst))))
			return;

		m_vecSubMonster.push_back(pMonst);

		CTransform* pMonsterTrans = pMonst->Get_Component<CTransform>();

		pMonsterTrans->Set_Scaling(0.16f, 0.16f, 0.16f);
		pMonsterTrans->Set_State(CTransform::STATE::STATE_POS, vPos[i]);

		dynamic_cast<CMonster*>(pMonst)->Set_Activate();

		if (i == 2)
			pMonst->Get_Component<CNavigation>()->Set_CurrentIndex(65);
		else
			pMonst->Get_Component<CNavigation>()->Set_CurrentIndex(58);
	}
}

_bool CHelicoScarrow::Is_SubMonster_AllDead()
{
	for(auto& iter : m_vecSubMonster)
	{
		if (!iter->Get_Dead())
			return false;
	}

	m_vecSubMonster.clear();

	return true;
}

void CHelicoScarrow::Dead_CountDown()
{
	m_iHited_Count[0] += 1;
}

void CHelicoScarrow::Creat_Bullet()
{
	CHelico_Bullet::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.1f;
	BulletDesc.fLifeTime = 1.f;
	BulletDesc.fSpeed = 6.f;
	BulletDesc.pTarget = m_pPlayer;

	_uint iNum = 0;

	if (m_bSelectBonNum)
	{
		iNum = 6;
		m_bSelectBonNum = false;
	}
	else {
		iNum = 12;
		m_bSelectBonNum = true;
	}

	CBone* pBone = m_pModelCom->Get_Bone(iNum);

	_matrix matWorld = pBone->Get_CombinedTransformationMatrix() *
		m_pTransformCom->Get_WorldMatrix_Matrix();

	_vector vBonePos;

	memcpy(&vBonePos, &matWorld.r[3], sizeof(_float4));

	XMStoreFloat4(&BulletDesc.fStartPos, vBonePos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		MODEL_HELICOBULLET_TAG, &BulletDesc)))
		return;
		
}

void CHelicoScarrow::Create_Shock_Wave()
{
	CShock_Wave::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.3f;
	BulletDesc.fLifeTime = 1.f;
	BulletDesc.fSpeed = 2.4f;
	BulletDesc.pTarget = nullptr;
	
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMVector3Normalize( m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK));

	_vector vWavePos = vPos + vLook * 0.1f;

	vWavePos.m128_f32[1] -= m_pTransformCom->Get_Scaled().y;

	XMStoreFloat4(&BulletDesc.fStartPos, vWavePos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_SHOCK_WAVE_TAG, &BulletDesc, reinterpret_cast<CGameObject**>(&m_pShockWave_Col))))
		return;

	_float4 vParticlePos;
	XMStoreFloat4(&vParticlePos, vPos);

	CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_BOSS1WAVE_TAG, GO_PARTICLEATTACK_TAG,
		this, vParticlePos, _float3(0.f, 0.f, 0.f), nullptr, 1.f);

	CUtility_Effect::Create_Effect_Reaper(m_pGameInstance, this, vPos, _float4(1.f, 0.6f, 0.4f, 1.f),
		&m_pShockWave_Effect, 1.f, _float2(8.f, 8.f));

	CUtility_Effect::Create_Damage_Effect(m_pGameInstance, this, 0.5f, _float2(1.5f, 1.5f));
}

void CHelicoScarrow::Shock_Wave_Radius_Compute()
{
	if (m_pShockWave_Col == nullptr || m_pShockWave_Col->Get_Dead())
		return;

	if (m_pShockWave_Effect == nullptr || m_pShockWave_Effect->Get_Dead())
		return;

	_vector vColPos = dynamic_cast<CShock_Wave*>(m_pShockWave_Col)->Get_ColWorldMat(1).r[3];
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	_vector vDir = vPos - vColPos;

	_float fDistance = XMVectorGetX( XMVector3Length(vDir));

	dynamic_cast<CEffect_Reaper*>(m_pShockWave_Effect)->Set_Radius(fDistance);
}

void CHelicoScarrow::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	if ((pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_PLAYER_BULLET) &&
		m_Status_Desc.bHited == false)
	{
		m_Status_Desc.bHited = true;
		m_bHit_Effect = true;
		Create_Damage_Effect(0.3f, TEX_DAMAGEIMPACT_TAG);
		m_iHited_Count[1] += 1;

		CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_BOSS1HIT_TAG,
			GO_PARTICLENORMAL_TAG,
			this, nullptr, 0.5f);

		m_pGameInstance->Play_Sound(L"HelicoScarrow", L"HitVoice.ogg", CHANNELID::SOUND_BOSS_VOICE, 0.5f);
		m_pGameInstance->Play_Sound(L"HelicoScarrow", L"Hit.ogg", CHANNELID::SOUND_BOSS_HIT, 0.5f);
	}
}

void CHelicoScarrow::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
	
}

void CHelicoScarrow::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CHelicoScarrow::Bind_ShaderResources()
{
	if (FAILED(CMonster::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHelicoScarrow::Ready_Component()
{
	if (FAILED(CMonster::Ready_Component()))
		return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.bUseCol = false;
	Sphere_Desc.fRadius = 0.7f;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CHelicoScarrow::Ready_State()
{

	if (FAILED(m_pStateMachineCom->Add_State(STATE::DEAD, CHelico_Dead::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DIVE, CHelico_Dive::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DIVE_IDLE, CHelico_Dive_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::EXHAUSTED_HIT, CHelico_Exhausted_Hit::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::EXHAUSTED_IDLE, CHelico_Exhausted_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::EXHAUSTED_WAKE, CHelico_Exhausted_Wake::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HELICO_ATTACK, CHelico_Attack::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HELICO_END, CHelico_Attack_End::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HELICO_START, CHelico_Attack_Start::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CHelico_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DIVE_UP, CHelico_Dive_Up::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::GROUND_SMASH, CHelico_Ground_Smash::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHelicoScarrow::Init_Point_Light()
{
	LIGHT_DESC LightDesc = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] = m_pTransformCom->Get_Scaled().y;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 0.7f;
	LightDesc.vDiffuse = _float4(1.f, 0.8f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 0.8f, 0.f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

	return S_OK;
}

CHelicoScarrow* CHelicoScarrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHelicoScarrow* pInstance = new CHelicoScarrow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHelicoScarrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHelicoScarrow::Clone(void* pArg)
{
	CHelicoScarrow* pInstance = new CHelicoScarrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHelicoScarrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelicoScarrow::Free()
{
	__super::Free();
	
}

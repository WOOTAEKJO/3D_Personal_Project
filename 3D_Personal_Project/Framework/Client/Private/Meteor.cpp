#include "stdafx.h"
#include "..\Public\Meteor.h"

#include "GameInstance.h"
#include "Character.h"

#include "Utility_Effect.h"
#include "Effect_Target.h"
#include "Particle.h"

CMeteor::CMeteor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CMeteor::CMeteor(const CMeteor& rhs)
	: CBullet(rhs)
{
}

HRESULT CMeteor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMeteor::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.5f, 0.5f, 0.5f);

	CEffect_Target::EFFECT_TARGETINFO Info = {};
	Info.pOwner = this;
	Info.fLifeTime = 0.f;
	Info.strEffectTextureTag = TEX_REAPER_TAG;
	Info.vSize = _float2(1.f, 1.f);
	Info.vColor = _float4(0.f, 1.f, 1.f, 1.f);
	
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_f32[1] = 7.1f;

	XMStoreFloat4(&Info.vPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTTARGET_TAG, &Info)))
		return E_FAIL;

	CUtility_Effect::Create_Effect_Reaper(m_pGameInstance, this, vPos, _float4(0.f, 1.f, 1.f, 1.f),
		nullptr, 1.5f, _float2(1.f, 1.f),CEffect_Reaper::COMPUTE_TYPE::POSY);

	/*CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_METEORTAIL_TAG, GO_PARTICLEALWAYS_TAG,
		this, nullptr, 0.05f);*/
	CGameObject* pEffect = nullptr;

	CParticle::PARTICLEINFO ParticleInfo = {};
	ParticleInfo.pOwner = this;
	ParticleInfo.strParticleTag = PARTICLE_METEORTAIL_TAG;
	ParticleInfo.fLifeTime = 0.f;
	ParticleInfo.pBones = ParticleInfo.pOwner->Get_Component<CModel>()->Get_Bones();

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_PARTICLEALWAYS_TAG, &ParticleInfo, reinterpret_cast<CGameObject**>(&pEffect))))
		return E_FAIL;

	CTransform* pTrans = pEffect->Get_Component<CTransform>();

	_vector vEffectPos = pTrans->Get_State(CTransform::STATE::STATE_POS);
	_vector vEffectRight = pTrans->Get_State(CTransform::STATE::STATE_RIGHT);

	vEffectPos.m128_f32[1] += 0.2f;

	pTrans->Set_State(CTransform::STATE::STATE_POS, vEffectPos);
	pTrans->Set_Scaling(2.f, 2.f, 2.f);

	return S_OK;
}

void CMeteor::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CMeteor::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_fTimeAcc>0.4f)
		Fall(fTimeDelta);
	
	__super::Tick(fTimeDelta);
}

void CMeteor::Late_Tick(_float fTimeDelta)
{
	/*if (m_pTransformCom->Get_State(CTransform::STATE::STATE_POS).m128_f32[1] < 7.f)
		Set_Dead();*/

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	if (XMVector3NearEqual(vPos, XMVectorSet(0.f, 7.f, 0.f, 0.f), XMVectorSet(1000.f, 0.1f, 1000.f, 0.f)))
	{
		_float4 vParticlePos;
		XMStoreFloat4(&vParticlePos, vPos);

		CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_METEORWAVE_TAG, GO_PARTICLEATTACK_TAG,
			this, vParticlePos, _float3(0.f, 0.f, 0.f), nullptr, 1.f);

		CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_METEORAFTER_TAG, GO_PARTICLENONBLUR_TAG,
			this, vParticlePos, _float3(0.f, 0.f, 0.f), nullptr, 1.7f);


		CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_METEOREXPLOSION1_TAG,
			GO_PARTICLEATTACK_TAG, this, vParticlePos, _float3(0.f, 0.f, 0.f), nullptr, 1.5f);
		CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_METEOREXPLOSION2_TAG,
			GO_PARTICLEATTACK_TAG, this, vParticlePos, _float3(0.f, 0.f, 0.f), nullptr, 1.5f);
		CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_METEOREXPLOSION3_TAG,
			GO_PARTICLEATTACK_TAG, this, vParticlePos, _float3(0.f, 0.f, 0.f), nullptr, 1.5f);

		Set_Dead();
	}
		

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
	
	__super::Late_Tick(fTimeDelta);

}

HRESULT CMeteor::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (_uint i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CMeteor::Fall(_float fTimeDelta)
{
	_vector vUp = m_pTransformCom->Get_State(CTransform::STATE::STATE_UP);

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	vPos += XMVector3Normalize(-vUp) * fTimeDelta * m_fSpeed;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
}

HRESULT CMeteor::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeteor::Ready_Component()
{
	if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(MODEL_BOMB_TAG, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

CMeteor* CMeteor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMeteor* pInstance = new CMeteor(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMeteor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMeteor::Clone(void* pArg)
{
	CMeteor* pInstance = new CMeteor(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMeteor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeteor::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}

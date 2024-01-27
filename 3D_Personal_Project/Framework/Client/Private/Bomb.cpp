#include "stdafx.h"
#include "..\Public\Bomb.h"

#include "GameInstance.h"
#include "Character.h"

#include "Utility_Effect.h"
#include "Particle_Always.h"

#include "Light.h"

CBomb::CBomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CBomb::CBomb(const CBomb& rhs)
	: CBullet(rhs)
{
}

HRESULT CBomb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBomb::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;

	if (FAILED(Init_Point_Light()))
		return E_FAIL;

	CGameObject* pEffect = nullptr;

	CParticle::PARTICLEINFO Info = {};
	Info.pOwner = this;
	Info.strParticleTag = PARTICLE_BOMBPARTICLE_TAG;
	Info.fLifeTime = 0.f;
	Info.pBones = Info.pOwner->Get_Component<CModel>()->Get_Bones();

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_PARTICLEALWAYS_TAG, &Info, reinterpret_cast<CGameObject**>(&pEffect))))
		return E_FAIL;

	CTransform* pTrans = pEffect->Get_Component<CTransform>();

	_vector vEffectPos = pTrans->Get_State(CTransform::STATE::STATE_POS);
	_vector vEffectRight = pTrans->Get_State(CTransform::STATE::STATE_RIGHT);

	vEffectPos += XMVector3Normalize(vEffectRight) * 0.144f;
	vEffectPos.m128_f32[1] += 0.2f;

	pTrans->Set_State(CTransform::STATE::STATE_POS, vEffectPos);

	return S_OK;
}

void CBomb::Priority_Tick(_float fTimeDelta)
{
	if (m_bStart)
		Start_Point_Toward(fTimeDelta);

	__super::Priority_Tick(fTimeDelta);
}

void CBomb::Tick(_float fTimeDelta)
{
	Color_Change(fTimeDelta);

	if (m_bStart) {
		if (m_pRigidBodyCom->Is_Power_Zero(CRigidBody::TYPE::TYPE_VELOCITY))
		{
			m_bFall = true;
		}
	}

	if (m_bFall && m_pTransformCom->Get_State(CTransform::STATE::STATE_POS).m128_f32[1] <= m_vTargetPos.y)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
		vPos.m128_f32[1] = (m_bBite == true ? 0.f : m_vTargetPos.y + 0.2f);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
		m_pTransformCom->Set_Ground(true);
		m_pRigidBodyCom->Reset_Force(CRigidBody::TYPE::TYPE_VELOCITY);
		m_bArrive = true;
		m_bFall = false;
		m_bStart = false;
	}

	__super::Tick(fTimeDelta);
}

void CBomb::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	/*if (m_bStart)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > 5.f)
			Set_Dead();
	}*/

	__super::Late_Tick(fTimeDelta);

}

HRESULT CBomb::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (_uint i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CBomb::Start_Point_Toward(_float fTimeDelta)
{
	if (m_bForce)
	{
		m_pTransformCom->LookAt(XMLoadFloat4(&m_vTargetPos));
		m_pTransformCom->Set_Ground(false);

		m_pRigidBodyCom->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fPower, CRigidBody::TYPE_VELOCITY);
		m_bForce = false;
	}

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	_vector vDir = XMVector3Normalize( XMLoadFloat4(&m_vTargetPos) - vPos);

	vPos = vPos + vDir * fTimeDelta * m_fSpeed;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	
}

void CBomb::Set_TartgetPos(_float4 vTargetPos)
{
	m_vTargetPos = vTargetPos;
}

void CBomb::Host_Bite()
{
	//XMStoreFloat4(&m_vTargetPos,m_pOwner->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS));
	_vector vOwnerPos = m_pOwner->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vDir = vOwnerPos - vPos;
	_float fLength = XMVectorGetX(XMVector3Length(vDir));

	_vector vPlayerPos = m_pTarget->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = vPos - vPlayerPos;
	vPos = vPos + XMVector3Normalize(vLook) * fLength;
	//vPos.m128_f32[1] = 0.f;
	XMStoreFloat4(&m_vTargetPos, vPos);

	m_bStart = true;
	m_bForce = true;
	m_bBite = true;
	m_fSpeed = 7.f;
}

void CBomb::Dead_Particle()
{
	_float4 vPos;
	XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE::STATE_POS));

	CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_EXPLOSION1_TAG,
		GO_PARTICLEATTACK_TAG, this, vPos, _float3(0.f, 0.f, 0.f), nullptr, 1.5f);
	CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_EXPLOSION2_TAG,
		GO_PARTICLEATTACK_TAG, this, vPos, _float3(0.f, 0.f, 0.f), nullptr, 1.5f);
	CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_EXPLOSION3_TAG,
		GO_PARTICLEATTACK_TAG, this, vPos, _float3(0.f, 0.f, 0.f), nullptr, 1.5f);
	CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_EXPLOSION4_TAG,
		GO_PARTICLEATTACK_TAG, this, vPos, _float3(0.f, 0.f, 0.f), nullptr, 1.5f);

	Set_Dead();
}

void CBomb::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_PLAYER_BULLET)
	{
		Host_Bite();

		m_bFailed = true;
	}

}

void CBomb::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void CBomb::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CBomb::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBomb::Ready_Component()
{
	if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(MODEL_BOMB_TAG, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

HRESULT CBomb::Init_Point_Light()
{
	LIGHT_DESC LightDesc = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] += m_pTransformCom->Get_Scaled().y;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 1.2f;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

	return S_OK;
}

void CBomb::Color_Change(_float fTimeDelta)
{
	_vector vColor = XMLoadFloat4(&m_vSolid_Color);

	if (m_vSolid_Color.y <= 0.f)
		m_iSign = 1.f;
	else if(m_vSolid_Color.y >= 1.f)
		m_iSign = -1.f;

		vColor.m128_f32[1] += fTimeDelta * m_iSign;

	XMStoreFloat4(&m_vSolid_Color, vColor);

	if (m_pLight != nullptr)
	{
		m_pLight->Open_Light_Desc()->vDiffuse = m_vSolid_Color;
		m_pLight->Open_Light_Desc()->vAmbient = m_vSolid_Color;
		m_pLight->Open_Light_Desc()->vSpecular = m_vSolid_Color;
	}

}

CBomb* CBomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBomb* pInstance = new CBomb(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBomb");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBomb::Clone(void* pArg)
{
	CBomb* pInstance = new CBomb(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBomb");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBomb::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}

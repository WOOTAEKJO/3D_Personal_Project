#include "stdafx.h"
#include "..\Public\Normal_Bullet.h"

#include "GameInstance.h"
#include "Character.h"

#include "Light.h"

#include "Utility_Effect.h"
#include "Effect_Trail.h"

CNormal_Bullet::CNormal_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CNormal_Bullet::CNormal_Bullet(const CNormal_Bullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CNormal_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNormal_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	BULLET_NORMAL_DESC* BulletDesc = (BULLET_NORMAL_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;

	m_pTransformCom->Set_Ground(false);

	m_pRigidBodyCom->Set_GravityPower(-2.f);
	
	m_pRigidBodyCom->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.f, CRigidBody::TYPE_VELOCITY);

	/*if (FAILED(Init_Point_Light()))
		return E_FAIL;*/

	CUtility_Effect::Create_Effect_Trail(m_pGameInstance, TEX_WATER_TAG, this, _float3(0.f, 0.03f, 0.f),
		_float3(0.f, 0.04f, 0.f), 10, 6, BulletDesc->vTrailColor, &m_pTrailEffect);

	return S_OK;
}

void CNormal_Bullet::Priority_Tick(_float fTimeDelta)
{
	if (m_pRigidBodyCom->Is_Power_Zero(CRigidBody::TYPE_VELOCITY))
	{
		m_bFall = true;
	}

	Toward(fTimeDelta);
	__super::Priority_Tick(fTimeDelta);
}

void CNormal_Bullet::Tick(_float fTimeDelta)
{
	/*if (m_bFall)
	{
		m_pRigidBodyCom->Set_GravityPower(-369.65f * fTimeDelta);
	}*/
	if(m_pTrailEffect != nullptr)
		dynamic_cast<CEffect_Trail*>(m_pTrailEffect)->Trail_Update(m_pTransformCom->Get_WorldMatrix_Matrix());
	__super::Tick(fTimeDelta);
}

void CNormal_Bullet::Late_Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > m_fLifeTime)
	{
		if(m_pLight != nullptr)
			m_pLight->Set_Active(false);
		Set_Dead();
	}

	__super::Late_Tick(fTimeDelta);

	Dead_Judge();
}

HRESULT CNormal_Bullet::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CNormal_Bullet::Toward(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMLoadFloat3(&m_vLook);

	vPos = vPos + vLook * fTimeDelta * m_fSpeed;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	//m_pRigidBodyCom->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), 18.f * fTimeDelta, CRigidBody::TYPE_VELOCITY);
	
}

void CNormal_Bullet::Dead_Judge()
{
	/*if (XMVector3NearEqual(XMLoadFloat4(&m_vTargetPos), m_pTransformCom->Get_State(CTransform::STATE::STATE_POS),
		XMVectorSet(0.1f, 0.1f, 0.1f, 0.f)))
	{
		Set_Dead();
	}*/
}

HRESULT CNormal_Bullet::Bind_ShaderResources()
{
	
	return S_OK;
}

HRESULT CNormal_Bullet::Ready_Component()
{
	if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNormal_Bullet::Init_Point_Light()
{
	LIGHT_DESC LightDesc = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] = m_pTransformCom->Get_Scaled().y;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 0.1f;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

	return S_OK;
}

CNormal_Bullet* CNormal_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNormal_Bullet* pInstance = new CNormal_Bullet(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNormal_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CNormal_Bullet::Clone(void* pArg)
{
	CNormal_Bullet* pInstance = new CNormal_Bullet(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNormal_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNormal_Bullet::Free()
{
	__super::Free();
}

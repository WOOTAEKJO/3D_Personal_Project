#include "stdafx.h"
#include "..\Public\Normal_Bullet.h"

#include "GameInstance.h"
#include "Character.h"

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


	m_pTransformCom->Set_Ground(false);

	m_pRigidBodyCom->Set_GravityPower(-2.f);
	
	m_pRigidBodyCom->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.f, CRigidBody::TYPE_VELOCITY);

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
	__super::Tick(fTimeDelta);
}

void CNormal_Bullet::Late_Tick(_float fTimeDelta)
{
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

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
}

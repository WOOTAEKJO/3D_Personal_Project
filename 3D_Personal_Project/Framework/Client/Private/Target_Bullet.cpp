#include "stdafx.h"
#include "..\Public\Target_Bullet.h"

#include "GameInstance.h"
#include "Character.h"

#include "Utility_Effect.h"
#include "Effect_Energy.h"

CTarget_Bullet::CTarget_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CTarget_Bullet::CTarget_Bullet(const CTarget_Bullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CTarget_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTarget_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;

	return S_OK;
}

void CTarget_Bullet::Priority_Tick(_float fTimeDelta)
{
	
	Toward(fTimeDelta);
	__super::Priority_Tick(fTimeDelta);
}

void CTarget_Bullet::Tick(_float fTimeDelta)
{
	
	__super::Tick(fTimeDelta);
}

void CTarget_Bullet::Late_Tick(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	if (XMVector3NearEqual(XMLoadFloat4(&m_vTargetPos), vPos,
		XMVectorSet(0.1f, 0.1f, 0.1f, 0.f)))
	{
		_float4 vParticlePos;
		XMStoreFloat4(&vParticlePos, vPos);

		CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_BOSS2SHOOTHIT_TAG,
			GO_PARTICLEATTACK_TAG, this, vParticlePos, _float3(0.f, 0.f, 0.f),nullptr,1.f);

		m_pGameInstance->Play_Sound(L"Phantom", L"Shoot.ogg", CHANNELID::SOUND_BOSS_ATTACK, 1.f);
		Set_Dead();
	}
		
	__super::Late_Tick(fTimeDelta);
}

HRESULT CTarget_Bullet::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTarget_Bullet::Toward(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMLoadFloat3(&m_vLook);

	vPos = vPos + vLook * fTimeDelta * m_fSpeed;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	
}


HRESULT CTarget_Bullet::Bind_ShaderResources()
{
	
	return S_OK;
}

HRESULT CTarget_Bullet::Ready_Component()
{
	if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

CTarget_Bullet* CTarget_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTarget_Bullet* pInstance = new CTarget_Bullet(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTarget_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTarget_Bullet::Clone(void* pArg)
{
	CTarget_Bullet* pInstance = new CTarget_Bullet(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTarget_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTarget_Bullet::Free()
{
	__super::Free();
}

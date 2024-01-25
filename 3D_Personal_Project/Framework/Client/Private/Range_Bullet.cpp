#include "stdafx.h"
#include "..\Public\Range_Bullet.h"

#include "GameInstance.h"
#include "Character.h"

CRange_Bullet::CRange_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CRange_Bullet::CRange_Bullet(const CRange_Bullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CRange_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRange_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;

	return S_OK;
}

void CRange_Bullet::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CRange_Bullet::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CRange_Bullet::Late_Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > m_fLifeTime)
		Set_Dead();

	__super::Late_Tick(fTimeDelta);
}

HRESULT CRange_Bullet::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRange_Bullet::Bind_ShaderResources()
{
	
	return S_OK;
}

HRESULT CRange_Bullet::Ready_Component()
{
	if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

CRange_Bullet* CRange_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRange_Bullet* pInstance = new CRange_Bullet(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRange_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CRange_Bullet::Clone(void* pArg)
{
	CRange_Bullet* pInstance = new CRange_Bullet(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRange_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRange_Bullet::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "..\Public\Bullet.h"

#include "GameInstance.h"

#include "Character.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CBullet::CBullet(const CBullet& rhs)
	:CGameObject(rhs)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	m_pOwner = BulletDesc->pOwner;
	m_fLifeTime = BulletDesc->fLifeTime;
	m_fSpeed = BulletDesc->fSpeed;
	m_fRadius = BulletDesc->fRadius;

	if (FAILED(__super::Initialize(BulletDesc)))
		return E_FAIL;

	/*if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;*/

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, BulletDesc->fStartPos);

	if (BulletDesc->pTarget != nullptr)
	{
		_vector vTargetPos = BulletDesc->pTarget->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS);
		XMStoreFloat4(&m_vTargetPos, vTargetPos);

		_vector vLook = XMVector3Normalize(vTargetPos - m_pTransformCom->Get_State(CTransform::STATE::STATE_POS));

		XMStoreFloat3(&m_vLook, vLook);
	}

	return S_OK;
}

void CBullet::Priority_Tick(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());

	CGameObject::Priority_Tick(fTimeDelta);
}

void CBullet::Tick(_float fTimeDelta)
{
	CGameObject::Tick(fTimeDelta);
}

void CBullet::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	CGameObject::Late_Tick(fTimeDelta);
}

HRESULT CBullet::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();

#endif

	return S_OK;
}

HRESULT CBullet::Bind_ShaderResources()
{
	
	/*if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
	->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos", 
		&m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS),sizeof(_float4))))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CBullet::Ready_Component()
{
	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.bUseCol = true;
	Sphere_Desc.fRadius = m_fRadius;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;

	CRigidBody::RIGIDBODY_DESC RigidBody_Desc = {};
	RigidBody_Desc.pOwner = this;
	if (FAILED(Add_Component<CRigidBody>(COM_RIGIDBODY_TAG, &m_pRigidBodyCom, &RigidBody_Desc))) return E_FAIL;

	return S_OK;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRigidBodyCom);
}

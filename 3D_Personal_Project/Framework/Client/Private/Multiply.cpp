#include "stdafx.h"
#include "..\Public\Multiply.h"

#include "GameInstance.h"
#include "Character.h"

#include "Bone.h"
#include "Model.h"

CMultiply::CMultiply(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CMultiply::CMultiply(const CMultiply& rhs)
	: CBullet(rhs)
{
}

HRESULT CMultiply::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMultiply::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;


	m_pSocketBone = m_pModelCom->Get_Bone(5);
	Safe_AddRef(m_pSocketBone);

	m_pModelCom->Set_AnimationIndex(1);

	m_pTransformCom->Set_Scaling(0.16f, 0.16f, 0.16f);

	m_pTransformCom->LookAt(m_pTarget->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS));

	return S_OK;
}

void CMultiply::Priority_Tick(_float fTimeDelta)
{

	m_pColliderCom->Update(m_pSocketBone->Get_CombinedTransformationMatrix() * m_pTransformCom->Get_WorldMatrix_Matrix());
	Toward(fTimeDelta);

}

void CMultiply::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, true);
	__super::Tick(fTimeDelta);
}

void CMultiply::Late_Tick(_float fTimeDelta)
{
	
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > m_fLifeTime)
		Set_Dead();

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	__super::Late_Tick(fTimeDelta);

}

HRESULT CMultiply::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CMultiply::Toward(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK));
	vLook.m128_f32[1] = 0.f;
	vPos = vPos + vLook * fTimeDelta * m_fSpeed;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	//m_pRigidBodyCom->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), 18.f * fTimeDelta, CRigidBody::TYPE_VELOCITY);
	
}

HRESULT CMultiply::Bind_ShaderResources()
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

HRESULT CMultiply::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(ANIMMODEL_PHANTOM_TAG, &m_pModelCom))) return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.bUseCol = true;
	Sphere_Desc.fRadius = 4.f;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius - 2.f, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;

	return S_OK;
}

CMultiply* CMultiply::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMultiply* pInstance = new CMultiply(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMultiply");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMultiply::Clone(void* pArg)
{
	CMultiply* pInstance = new CMultiply(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMultiply");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMultiply::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pSocketBone);
}

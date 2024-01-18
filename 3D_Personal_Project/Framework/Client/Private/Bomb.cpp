#include "stdafx.h"
#include "..\Public\Bomb.h"

#include "GameInstance.h"
#include "Character.h"

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
	if (m_bStart) {
		if (m_pRigidBodyCom->Is_Power_Zero(CRigidBody::TYPE::TYPE_VELOCITY))
		{
			m_bFall = true;
		}
	}

	if (m_bFall && m_pTransformCom->Get_State(CTransform::STATE::STATE_POS).m128_f32[1] <= m_vTargetPos.y)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
		vPos.m128_f32[1] = 7.2f;
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

		m_pShaderCom->Begin(0);

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
		m_pTransformCom->Set_Ground(false);

		m_pRigidBodyCom->Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), 15.f, CRigidBody::TYPE_VELOCITY);
		m_bForce = false;
	}

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	_vector vDir = XMVector3Normalize( XMLoadFloat4(&m_vTargetPos) - vPos);

	vPos = vPos + vDir * fTimeDelta * m_fSpeed;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	m_pTransformCom->LookAt(XMLoadFloat4(&m_vTargetPos));
}

void CBomb::Set_TartgetPos(_float4 vTargetPos)
{
	m_vTargetPos = vTargetPos;
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

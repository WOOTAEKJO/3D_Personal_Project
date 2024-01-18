#include "stdafx.h"
#include "..\Public\Meteor.h"

#include "GameInstance.h"
#include "Character.h"

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

	return S_OK;
}

void CMeteor::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CMeteor::Tick(_float fTimeDelta)
{
	Fall(fTimeDelta);
	__super::Tick(fTimeDelta);
}

void CMeteor::Late_Tick(_float fTimeDelta)
{
	/*if (m_pTransformCom->Get_State(CTransform::STATE::STATE_POS).m128_f32[1] < 7.f)
		Set_Dead();*/

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

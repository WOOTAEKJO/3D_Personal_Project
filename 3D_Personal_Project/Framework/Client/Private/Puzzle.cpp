#include "stdafx.h"
#include "..\Public\Puzzle.h"

#include "GameInstance.h"

#include "Light.h"

CPuzzle::CPuzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CPuzzle::CPuzzle(const CPuzzle& rhs)
	:CGameObject(rhs)
{
}

HRESULT CPuzzle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPuzzle::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	PUZZLE_DESC* pTriggerDesc = (PUZZLE_DESC*)pArg;

	m_strModelTag = pTriggerDesc->strModelTag;
	//m_vLightColor = pTriggerDesc->vLightColor;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_TRIGGER_BULLET, m_pColliderCom)))
		return E_FAIL;

	return S_OK;
}

void CPuzzle::Priority_Tick(_float fTimeDelta)
{
	/*if (m_pLight == nullptr)
	{
		Init_Point_Light();
	}*/

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());
}

void CPuzzle::Tick(_float fTimeDelta)
{
	if (!m_bFinish && m_bTrue)
	{
		Down(fTimeDelta);
	}
}

void CPuzzle::Late_Tick(_float fTimeDelta)
{
	Reset_Hit(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
	if (FAILED(m_pGameInstance->Add_DebugRender(m_pColliderCom)))
		return;
}

HRESULT CPuzzle::Render()
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

	return S_OK;
}

void CPuzzle::Down(_float fTimeDelta)
{

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	if (XMVector3NearEqual(vPos, XMVectorSet(0.f, 3.f, 0.f, 0.f), XMVectorSet(1000.f, 0.1f, 1000.f, 1.f)))
	{
		m_bFinish = true;
		return;
	}

	_vector vUp = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_UP));

	vPos += vUp * fTimeDelta * -1.f;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);

}

void CPuzzle::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{

	if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_PLAYER_BULLET
		&& m_bHit == false && !m_bRelent)
	{
		m_bHit = true;
		m_bRelent = true;
	}
}

void CPuzzle::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void CPuzzle::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CPuzzle::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	/*if (m_bHit && m_bRelent)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vLightColor, sizeof(_float4))))
			return E_FAIL;
	}
	else {
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vResetColor, sizeof(_float4))))
			return E_FAIL;
	}*/

	if (m_bRelent)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vLightColor, sizeof(_float4))))
			return E_FAIL;
	}
	else {
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vResetColor, sizeof(_float4))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPuzzle::Ready_Component()
{
	CBounding_AABB::BOUNDING_AABB_DESC AABB_Desc = {};
	AABB_Desc.pOnwer = this;
	AABB_Desc.eType = CBounding::TYPE::TYPE_AABB;
	AABB_Desc.bUseCol = true;
	AABB_Desc.vExtents = _float3(0.1f, 0.2f, 0.1f);
	AABB_Desc.vCenter = _float3(0.f, AABB_Desc.vExtents.y, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom,&AABB_Desc))) return E_FAIL;

	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

HRESULT CPuzzle::Init_Point_Light()
{
	LIGHT_DESC LightDesc = {};

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_f32[1] += m_pTransformCom->Get_Scaled().y;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPos = m_vLightPos;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 5.f;
	LightDesc.vDiffuse = m_vLightColor;//_float4(0.8f, 0.8f, 0.f, 1.f);
	LightDesc.vAmbient = m_vLightColor;//_float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

	return S_OK;
}

void CPuzzle::Reset_Hit(_float fTimeDelta)
{
	if (m_bRelent && !m_bHit) {
		m_fTimeAcc += fTimeDelta;

		if (m_fTimeAcc > 1.f)
		{
			m_bRelent = false;
			m_fTimeAcc = 0.f;
		}
	}
}

CPuzzle* CPuzzle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPuzzle* pInstance = new CPuzzle(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPuzzle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPuzzle::Clone(void* pArg)
{
	CPuzzle* pInstance = new CPuzzle(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPuzzle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPuzzle::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pLight);

}

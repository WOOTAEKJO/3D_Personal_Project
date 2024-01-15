#include "stdafx.h"
#include "Player_Weapon_Spear.h"

#include "GameInstance.h"

#include "Bone.h"

#include "Player_IDLE.h"
#include "Player_Run.h"
#include "Player_Spear_AirAttack.h"
#include "Player_Jump.h"

CPlayer_Weapon_Spear::CPlayer_Weapon_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CPlayer_Weapon_Spear::CPlayer_Weapon_Spear(const CPlayer_Weapon_Spear& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer_Weapon_Spear::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Weapon_Spear::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/*if (FAILED(Ready_Component()))
		return E_FAIL;*/

	if (FAILED(m_pGameInstance->Load_Data_Json(GO_PLAYER_SPEAR_TAG,this)))
		return E_FAIL;

	m_pParentsTransform = ((PLAYERSPEAR_DESC*)pArg)->pParentsTransform;
	Safe_AddRef(m_pParentsTransform);
	m_pSocketBone = (((PLAYERSPEAR_DESC*)pArg)->pBones)[m_iSocketBoneIndex];
	Safe_AddRef(m_pSocketBone);

	m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_PLAYER_BULLET, m_pColliderCom);

	return S_OK;
}

void CPlayer_Weapon_Spear::Priority_Tick(_float fTimeDelta)
{

	m_pColliderCom->Update(XMLoadFloat4x4(&m_matWorldMat));
}

void CPlayer_Weapon_Spear::Tick(_float fTimeDelta)
{
	
}

void CPlayer_Weapon_Spear::Late_Tick(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_matWorldMat ,m_pTransformCom->Get_WorldMatrix_Matrix() *m_pSocketBone->Get_CombinedTransformationMatrix() * m_pParentsTransform->Get_WorldMatrix_Matrix());

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CPlayer_Weapon_Spear::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif


	return S_OK;
}

void CPlayer_Weapon_Spear::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("ModelTag") == In_Json.end())
		return;

	m_iSocketBoneIndex = In_Json["BoneIndex"];
	string strTag = In_Json["ModelTag"];

	m_strModelTag.assign(strTag.begin(), strTag.end());

	CGameObject::Load_FromJson(In_Json);

	if (FAILED(Ready_Component()))
		return;
}

void CPlayer_Weapon_Spear::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	/*for (_uint i = 0; i < (_uint)COL_END; i++)
	{
		m_pColliderCom[i]->Set_UseCol(false);
	}*/
}

void CPlayer_Weapon_Spear::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void CPlayer_Weapon_Spear::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CPlayer_Weapon_Spear::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorldMat )))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Weapon_Spear::Ready_Component()
{

	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.fRadius = 20.f;
	Sphere_Desc.vCenter = _float3(0.f, 190.f, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;
	for (_uint i = 1; i < 4; i++)
	{
		Sphere_Desc.vCenter = _float3(0.f, 190.f - (40.f*i), 0.f);
		m_pColliderCom->Add_Bounding(&Sphere_Desc);
	}
	

	return S_OK;
}

CPlayer_Weapon_Spear* CPlayer_Weapon_Spear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Weapon_Spear* pInstance = new CPlayer_Weapon_Spear(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Weapon_Spear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Weapon_Spear::Clone(void* pArg)
{
	CPlayer_Weapon_Spear* pInstance = new CPlayer_Weapon_Spear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Weapon_Spear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Weapon_Spear::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pSocketBone);
	Safe_Release(m_pParentsTransform);

	Safe_Release(m_pColliderCom);

}

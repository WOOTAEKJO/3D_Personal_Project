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

	/*if (FAILED(Load_Data((CUtility_String::WString_To_string(m_pGameInstance->
		PathFinder(GO_PLAYER_SPEAR_TAG,CSaveLoad_Manager::TYPE_DATA)).c_str()))))
		return E_FAIL;*/

	if (FAILED(Load_Data(m_pGameInstance->Load_Data_Path(GO_PLAYER_SPEAR_TAG).c_str())))
		return E_FAIL;

	m_pParentsTransform = ((PLAYERSPEAR_DESC*)pArg)->pParentsTransform;
	Safe_AddRef(m_pParentsTransform);
	m_pSocketBone = (((PLAYERSPEAR_DESC*)pArg)->pBones)[m_iSocketBoneIndex];
	Safe_AddRef(m_pSocketBone);

	return S_OK;
}

void CPlayer_Weapon_Spear::Priority_Tick(_float fTimeDelta)
{
	
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

		m_pShaderCom->Begin(SHADER_TBN::TBN_MODEL);

		m_pModelCom->Render(i);
	}

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

	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos", &m_pGameInstance->Get_Camera_Pos(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Weapon_Spear::Ready_Component()
{

	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Weapon_Spear::Load_Data(const _char* strFilePath)
{
	json jLoad;

	if (FAILED(CJson_Utility::Load_Json(strFilePath, jLoad)))
		return E_FAIL;

	Load_FromJson(jLoad);

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

}

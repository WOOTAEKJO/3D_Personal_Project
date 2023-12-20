#include "stdafx.h"
#include "..\Public\SubObject_Demo.h"

#include "Bone.h"

CSubObject_Demo::CSubObject_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CDemo(pDevice, pContext)
{
}

CSubObject_Demo::CSubObject_Demo(const CSubObject_Demo& rhs)
	:CDemo(rhs)
{
}

HRESULT CSubObject_Demo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSubObject_Demo::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr) {

		SUBOBJECTDEMO_DESC* SubObjectDemo_Desc = (SUBOBJECTDEMO_DESC*)pArg;

		m_pParentsTransform = SubObjectDemo_Desc->pParentsTransform;
		m_strModelTag = SubObjectDemo_Desc->strModelTag;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, SubObjectDemo_Desc->vPos);

		if (FAILED(Ready_Component()))
			return E_FAIL;
	}
	
	return S_OK;
}

void CSubObject_Demo::Priority_Tick(_float fTimeDelta)
{
}

void CSubObject_Demo::Tick(_float fTimeDelta)
{
	
}

void CSubObject_Demo::Late_Tick(_float fTimeDelta)
{
	if (m_pSocketBone != nullptr)
		XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() * m_pSocketBone->Get_CombinedTransformationMatrix() * m_pParentsTransform->Get_WorldMatrix_Matrix());

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CSubObject_Demo::Render()
{
	if (m_pModelCom == nullptr)
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (_uint i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(SHADER_TBN::TBN_MODEL);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CSubObject_Demo::Set_Control_Variable(void* pArg)
{
	/*if (pArg == nullptr)
		return E_FAIL;

	OBDEMOVALUE* ObjectDemoValue = (OBDEMOVALUE*)pArg;

	m_vObjectPos = ObjectDemoValue->vPos;*/

	return S_OK;
}

void CSubObject_Demo::Set_SocketBone(CBone* pBone)
{
	if (pBone == nullptr)
		return;

	m_pSocketBone = pBone;
}

_bool CSubObject_Demo::Get_Picked()
{
	if (m_pModelCom == nullptr || 
		m_pTransformCom==nullptr)
		return false;

	_float3 vPickPos;

	if (m_pModelCom->Compute_MousePos(&vPickPos, m_pTransformCom->Get_WorldMatrix_Matrix()))
		return true;
	
	return false;
}

void CSubObject_Demo::Write_Json(json& Out_Json)
{
	string strTag;

	strTag.assign(m_strModelTag.begin(), m_strModelTag.end());

	Out_Json.emplace("ModelTag", strTag);
	
	CGameObject::Write_Json(Out_Json);
}

void CSubObject_Demo::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("ModelTag") == In_Json.end())
		return;

	string strTag  = In_Json["ModelTag"];

	m_strModelTag.assign(strTag.begin(), strTag.end());

	CGameObject::Load_FromJson(In_Json);

	if (FAILED(Ready_Component()))
		return;
}

HRESULT CSubObject_Demo::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorldMat)))
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

HRESULT CSubObject_Demo::Ready_Component()
{

	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

CSubObject_Demo* CSubObject_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSubObject_Demo* pInstance = new CSubObject_Demo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSubObject_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSubObject_Demo::Clone(void* pArg)
{
	CSubObject_Demo* pInstance = new CSubObject_Demo(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSubObject_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSubObject_Demo::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	/*Safe_Release(m_pSocketBone);
	Safe_Release(m_pParentsTransform);*/
}

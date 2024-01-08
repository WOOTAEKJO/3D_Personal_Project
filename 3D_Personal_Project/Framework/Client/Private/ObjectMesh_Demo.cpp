#include "stdafx.h"
#include "..\Public\ObjectMesh_Demo.h"

CObjectMesh_Demo::CObjectMesh_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CDemo(pDevice, pContext)
{
}

CObjectMesh_Demo::CObjectMesh_Demo(const CObjectMesh_Demo& rhs)
	:CDemo(rhs)
{
}

HRESULT CObjectMesh_Demo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CObjectMesh_Demo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		OBDEMOVALUE* ObjectDemoValue = (OBDEMOVALUE*)pArg;

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, ObjectDemoValue->vPos);
		m_strModelTag = ObjectDemoValue->strModelTag;

		if (FAILED(Ready_Component()))
			return E_FAIL;
	}
	
	return S_OK;
}

void CObjectMesh_Demo::Priority_Tick(_float fTimeDelta)
{
}

void CObjectMesh_Demo::Tick(_float fTimeDelta)
{
	
}

void CObjectMesh_Demo::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CObjectMesh_Demo::Render()
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

HRESULT CObjectMesh_Demo::Set_Control_Variable(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	OBDEMOVALUE* ObjectDemoValue = (OBDEMOVALUE*)pArg;

	m_vObjectPos = ObjectDemoValue->vPos;

	return S_OK;
}

void CObjectMesh_Demo::Set_TransformState(CTransform::STATE eType, _float4 vVector)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Set_State(eType, vVector);
}

_float4 CObjectMesh_Demo::Get_TransformState(CTransform::STATE eType)
{
	if (m_pTransformCom == nullptr)
		return _float4();

	_float4 vVec;
	XMStoreFloat4(&vVec, m_pTransformCom->Get_State(eType));

	return vVec;
}

void CObjectMesh_Demo::Rotation(_float fX, _float fY, _float fZ)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Rotation_Total(XMConvertToRadians(fX),
		XMConvertToRadians(fY),
		XMConvertToRadians(fZ));
}

void CObjectMesh_Demo::Set_Scale(_float fX, _float fY, _float fZ)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Set_Scaling(fX, fY, fZ);
}

_bool CObjectMesh_Demo::Get_Picked(_float4* vOutPos)
{
	if (m_pModelCom == nullptr || 
		m_pTransformCom==nullptr)
		return false;

	_float3 vPickPos;

	//m_pGameInstance->Update_Mouse();

	if (m_pModelCom->Compute_MousePos(&vPickPos, m_pTransformCom->Get_WorldMatrix_Matrix()))
	{
		XMStoreFloat4(vOutPos, XMVector3TransformCoord(XMLoadFloat3(&vPickPos), m_pTransformCom->Get_WorldMatrix_Matrix()));

		//*vOutPos = _float4(vPickPos.x, vPickPos.y, vPickPos.z, 1.f);

		return true;
	}

	return false;
}

_bool CObjectMesh_Demo::Get_Picked_Dist(_float4* vOutPos, _float* fDist)
{
	if (m_pModelCom == nullptr ||
		m_pTransformCom == nullptr)
		return false;

	_float3 vPickPos;

	if (m_pModelCom->Compute_MousePos_Dist(&vPickPos,m_pTransformCom->Get_WorldMatrix_Matrix(), fDist))
	{
		XMStoreFloat4(vOutPos, XMVector3TransformCoord(XMLoadFloat3(&vPickPos), m_pTransformCom->Get_WorldMatrix_Matrix()));

		return true;
	}

	return false;
}

void CObjectMesh_Demo::Write_Json(json& Out_Json)
{
	string strTag;

	strTag.assign(m_strModelTag.begin(), m_strModelTag.end());

	Out_Json.emplace("ModelTag", strTag);
	CGameObject::Write_Json(Out_Json);
}

void CObjectMesh_Demo::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("ModelTag") == In_Json.end())
		return;

	string strTag  = In_Json["ModelTag"];

	m_strModelTag.assign(strTag.begin(), strTag.end());

	CGameObject::Load_FromJson(In_Json);

	if (FAILED(Ready_Component()))
		return;
}

HRESULT CObjectMesh_Demo::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos",
		&m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjectMesh_Demo::Ready_Component()
{
	
	/* For.Com_Shader*/ 
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), SHADER_MESH_TAG,
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), m_strModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CObjectMesh_Demo* CObjectMesh_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CObjectMesh_Demo* pInstance = new CObjectMesh_Demo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CObjectMesh_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CObjectMesh_Demo::Clone(void* pArg)
{
	CObjectMesh_Demo* pInstance = new CObjectMesh_Demo(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CObjectMesh_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CObjectMesh_Demo::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

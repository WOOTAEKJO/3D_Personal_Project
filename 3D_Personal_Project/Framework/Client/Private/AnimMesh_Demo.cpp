#include "stdafx.h"
#include "..\Public\AnimMesh_Demo.h"

CAnimMesh_Demo::CAnimMesh_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CDemo(pDevice, pContext)
{
}

CAnimMesh_Demo::CAnimMesh_Demo(const CAnimMesh_Demo& rhs)
	:CDemo(rhs)
{
}

HRESULT CAnimMesh_Demo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimMesh_Demo::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	ANIMDEMOVALUE* ObjectDemoValue = (ANIMDEMOVALUE*)pArg;
	
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, ObjectDemoValue->vPos);
	m_strModelTag = ObjectDemoValue->strModelTag;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CAnimMesh_Demo::Priority_Tick(_float fTimeDelta)
{
}

void CAnimMesh_Demo::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta,true);
}

void CAnimMesh_Demo::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CAnimMesh_Demo::Render()
{
	if (m_pModelCom == nullptr)
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		m_pShaderCom->Begin(SHADER_TBN::TBN_MODEL);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CAnimMesh_Demo::Set_Control_Variable(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	ANIMDEMOVALUE* ObjectDemoValue = (ANIMDEMOVALUE*)pArg;

	m_vObjectPos = ObjectDemoValue->vPos;

	return S_OK;
}

void CAnimMesh_Demo::Set_TransformState(CTransform::STATE eType, _float4 vVector)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Set_State(eType, vVector);
}

_float4 CAnimMesh_Demo::Get_TransformState(CTransform::STATE eType)
{
	if (m_pTransformCom == nullptr)
		return _float4();

	_float4 vVec;
	XMStoreFloat4(&vVec, m_pTransformCom->Get_State(eType));

	return vVec;
}

void CAnimMesh_Demo::Rotation(_float fX, _float fY, _float fZ)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Rotation_Total(XMConvertToRadians(fX),
		XMConvertToRadians(fY),
		XMConvertToRadians(fZ));
}

void CAnimMesh_Demo::Set_Scale(_float fX, _float fY, _float fZ)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Set_Scaling(fX, fY, fZ);
}

_bool CAnimMesh_Demo::Get_Picked()
{
	if (m_pModelCom == nullptr || 
		m_pTransformCom==nullptr)
		return false;

	_float3 vPickPos;

	//m_pGameInstance->Update_Mouse();

	if (m_pModelCom->Compute_MousePos(&vPickPos, m_pTransformCom->Get_WorldMatrix_Matrix()))
		return true;
	
	return false;
}

HRESULT CAnimMesh_Demo::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
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

HRESULT CAnimMesh_Demo::Ready_Component()
{

	/* For.Com_Shader*/ 
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), SHADER_ANIMMESH_TAG,
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Add_Component(m_pGameInstance->Get_Current_Level(), m_strModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CAnimMesh_Demo* CAnimMesh_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAnimMesh_Demo* pInstance = new CAnimMesh_Demo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAnimMesh_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CAnimMesh_Demo::Clone(void* pArg)
{
	CAnimMesh_Demo* pInstance = new CAnimMesh_Demo(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAnimMesh_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimMesh_Demo::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

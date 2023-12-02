#include "stdafx.h"
#include "..\Public\ObjectMesh_Demo.h"

#include "GameInstance.h"

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
	if (pArg == nullptr)
		return E_FAIL;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	OBDEMOVALUE* ObjectDemoValue = (OBDEMOVALUE*)pArg;
	
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, ObjectDemoValue->vPos);
	m_strModelTag = ObjectDemoValue->strModelTag;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;

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

	m_pShaderCom->Begin(2);

	m_pModelCom->Render();

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

	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos", &m_pGameInstance->Get_Camera_Pos(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjectMesh_Demo::Ready_Component()
{
	
	/* For.Com_Shader*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VTXTBN"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(Add_Component(LEVEL_TOOL, m_strModelTag,
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

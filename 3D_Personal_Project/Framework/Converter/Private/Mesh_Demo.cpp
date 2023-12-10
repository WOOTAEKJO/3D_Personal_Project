#include "stdafx.h"
#include "..\Public\Mesh_Demo.h"

#include "Converter_Model.h"

CMesh_Demo::CMesh_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CMesh_Demo::CMesh_Demo(const CMesh_Demo& rhs)
	:CGameObject(rhs)
{
}

HRESULT CMesh_Demo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMesh_Demo::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	OBDEMOVALUE* ObjectDemoValue = (OBDEMOVALUE*)pArg;

	m_strModelTag = ObjectDemoValue->strModelTag;


	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CMesh_Demo::Priority_Tick(_float fTimeDelta)
{
}

void CMesh_Demo::Tick(_float fTimeDelta)
{
}

void CMesh_Demo::Late_Tick(_float fTimeDelta)
{
}

HRESULT CMesh_Demo::Render()
{
	return S_OK;
}

HRESULT CMesh_Demo::Binarization(const _char* strPath)
{
	if (m_pModelCom == nullptr)
		return E_FAIL;

	if (FAILED(m_pModelCom->Set_Buffer(strPath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMesh_Demo::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CMesh_Demo::Ready_Component()
{
	/* For.Com_Model*/
	if (FAILED(Add_Component(LEVET_CONVERTER, m_strModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	

	return S_OK;
}

CMesh_Demo* CMesh_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMesh_Demo* pInstance = new CMesh_Demo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMesh_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMesh_Demo::Clone(void* pArg)
{
	CMesh_Demo* pInstance = new CMesh_Demo(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMesh_Demo::Free()
{
	__super::Free();
	
	Safe_Release(m_pModelCom);
}

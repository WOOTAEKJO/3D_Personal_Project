#include "..\Public\Navigation.h"
#include "GameInstance.h"

#include "Shader.h"
#include "Cell.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs)
	:CComponent(rhs),
	m_vecCell(rhs.m_vecCell)
#ifdef _DEBUG
	,m_pShaderCom(rhs.m_pShaderCom)
#endif
{
	for (auto& iter : m_vecCell)
		Safe_AddRef(iter);

#ifdef _DEBUG

	Safe_AddRef(m_pShaderCom);

#endif

}

HRESULT CNavigation::Initialize_ProtoType(const _char* strNavigationPath)
{
	ifstream ifs;

	ifs.open(strNavigationPath);

	if (ifs.is_open())
	{
		_uint iSize = 0;
		ifs.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
		for (_uint i = 0; i < iSize; i++)
		{
			FLOAT3X3 Float33;
			ifs.read(reinterpret_cast<char*>(&Float33), sizeof(FLOAT3X3));

			CCell* pCell = nullptr;
			pCell = CCell::Create(m_pDevice, m_pContext, Float33);
			if (pCell == nullptr)
				return E_FAIL;
			
			m_vecCell.push_back(pCell);
		}
	}

#ifdef _DEBUG
	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNavigation.hlsl"),
		VTXPOS::Elements, VTXPOS::iElementsNum);
	if (m_pShaderCom == nullptr)
		return E_FAIL;
#endif
	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CNavigation::Render()
{

#ifdef _DEBUG
	_float4x4 matView = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW);
	_float4x4 matProj = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ);

	for (auto& iter : m_vecCell)
	{
		if (iter != nullptr)
			iter->Render(m_pShaderCom, matView, matProj);
	}
#endif

	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* strNavigationPath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(strNavigationPath))) {
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& iter : m_vecCell)
		Safe_Release(iter);
	m_vecCell.clear();

#ifdef _DEBUG
	Safe_Release(m_pShaderCom);
#endif
}

#include "..\Public\Navigation.h"
#include "GameInstance.h"

#include "Shader.h"
#include "Cell.h"

#include "MeshData.h"

_float4x4 CNavigation::m_matWorld = {};

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

HRESULT CNavigation::Initialize_ProtoType(_bool bLoad,const _char* strNavigationPath)
{
	if (bLoad) {
		File_Load(strNavigationPath);
		if (FAILED(Init_Neighbor()))
			return E_FAIL;
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

	_float4 vColor = {};

	if (m_iCurrentCellIndex == -1)
		vColor = { 0.f,1.f,0.f,1.f };
	else {
		vColor = { 1.f,0.f,0.f,1.f };
		m_matWorld.m[3][1] += 0.1f;
	}

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorld)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &matView)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &matProj)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	if (m_iCurrentCellIndex != -1)
	{
		m_vecCell[m_iCurrentCellIndex]->Render();

	}
	else {
		for (auto& iter : m_vecCell)
		{
			if (iter != nullptr)
				iter->Render();
		}
	}
#endif

	return S_OK;
}

void CNavigation::Update(_float4x4 matWorld)
{
	m_matWorld = matWorld;
}

_bool CNavigation::IsMove(_fvector vPosition)
{
	_int iNeighborIndex = -1;

	if (m_vecCell[m_iCurrentCellIndex]->IsIn(vPosition,XMLoadFloat4x4(&m_matWorld), &iNeighborIndex))
		return true;
	else 
	{
		if (iNeighborIndex != -1)
		{

			if (iNeighborIndex == -1)
				return false;

			while (true)
			{
				if (m_vecCell[iNeighborIndex]->IsIn(vPosition, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex))
				{
					m_iCurrentCellIndex = iNeighborIndex;
					return true;
				}
			}

		}
		else {
			return false;
		}
	}
}

HRESULT CNavigation::Add_Cell(_float3* pPoints)
{
	FLOAT3X3 float33 = {};
	float33.vVertex0 = pPoints[0];
	float33.vVertex1 = pPoints[1];
	float33.vVertex2 = pPoints[2];

	CCell* pCell = CCell::Create(m_pDevice, m_pContext, float33);
	if (pCell == nullptr)
		return E_FAIL;

	m_vecCell.push_back(pCell);

	if (FAILED(Init_Neighbor()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::Save_Navigation(const _char* strFilePath)
{
	
	CMeshData::MESHDATADESC MeshDataDesc = {};

	MeshDataDesc.eModel_Type = CMeshData::MODEL_TYPE::NAVIGATION;
	
	for (auto& iter : m_vecCell)
	{
		FLOAT3X3	Float33 = {};

		Float33.vVertex0 = iter->Get_Point(CCell::POINTS::POINT_A);
		Float33.vVertex1 = iter->Get_Point(CCell::POINTS::POINT_B);
		Float33.vVertex2 = iter->Get_Point(CCell::POINTS::POINT_C);

		MeshDataDesc.vecNaviPoints.push_back(Float33);
	}

	if (FAILED(m_pGameInstance->Save_Data_Mesh(strFilePath, MeshDataDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::Init_Neighbor()
{
	for (auto& SourCell : m_vecCell)
	{
		for (auto& DestCell : m_vecCell)
		{
			if (DestCell->Compare_Points(SourCell->Get_Point(CCell::POINTS::POINT_A), SourCell->Get_Point(CCell::POINTS::POINT_B)))
			{
				DestCell->Set_NeighborIndex(CCell::LINES::LINE_AB, SourCell->Get_Index());
			}
			if (DestCell->Compare_Points(SourCell->Get_Point(CCell::POINTS::POINT_B), SourCell->Get_Point(CCell::POINTS::POINT_C)))
			{
				DestCell->Set_NeighborIndex(CCell::LINES::LINE_BC, SourCell->Get_Index());
			}
			if (DestCell->Compare_Points(SourCell->Get_Point(CCell::POINTS::POINT_C), SourCell->Get_Point(CCell::POINTS::POINT_A)))
			{
				DestCell->Set_NeighborIndex(CCell::LINES::LINE_CA, SourCell->Get_Index());
			}
		}
	}

	return S_OK;
}

HRESULT CNavigation::File_Load(const _char* strNavigationPath)
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

	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _bool bLoad, const _char* strNavigationPath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(bLoad,strNavigationPath))) {
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

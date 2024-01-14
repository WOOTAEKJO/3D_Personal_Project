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
	, m_eNaviType(rhs.m_eNaviType)
{
	for (auto& iter : m_vecCell)
		Safe_AddRef(iter);

#ifdef _DEBUG

	Safe_AddRef(m_pShaderCom);

#endif

}

HRESULT CNavigation::Initialize_ProtoType(NAVITYPE eType,const _char* strNavigationPath)
{
	m_eNaviType = eType;

	if (m_eNaviType == NAVITYPE::TYPE_LOAD) {
		File_Load(strNavigationPath);
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
	if (pArg != nullptr)
	{
		m_iCurrentCellIndex = ((NAVIGATION_DESC*)pArg)->iCurrentIndex;
	}

	return S_OK;
}

HRESULT CNavigation::Render()
{

#ifdef _DEBUG

	_float4x4 matView = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW);
	_float4x4 matProj = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ);

	_float4 vColor = {};

	/*if (m_iCurrentCellIndex == -1)
		vColor = { 0.f,1.f,0.f,1.f };
	else {

		vColor = { 1.f,0.f,0.f,1.f };
		m_matWorld.m[3][1] += 0.1f;
	}*/

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorld)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &matView)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &matProj)))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_float4))))
		return E_FAIL;*/

	//m_pShaderCom->Begin(0);

	if (m_iCurrentCellIndex != -1)
	{
		vColor = { 1.f,0.f,0.f,1.f };
		m_matWorld.m[3][1] += 0.1f;
		m_vecCell[m_iCurrentCellIndex]->Render(m_pShaderCom, vColor);

	}
	else {
		for (auto& iter : m_vecCell)
		{
			if (iter != nullptr)
			{
				if(iter->Get_CellType() == CCell::CELLTYPE::TYPE_NORMAL)
					vColor = { 0.f,1.f,0.f,1.f };
				else if(iter->Get_CellType() == CCell::CELLTYPE::TYPE_JUMP)
					vColor = { 0.f,0.f,1.f,1.f };

				iter->Render(m_pShaderCom, vColor);
			}
				
		}
	}
#endif

	return S_OK;
}

void CNavigation::Update(_float4x4 matWorld)
{
	m_matWorld = matWorld;
}

_bool CNavigation::IsMove(_fvector vPosition, _Out_ _float3* vLine)
{
	_int iNeighborIndex = -1;

	if (m_vecCell[m_iCurrentCellIndex]->IsIn(vPosition,XMLoadFloat4x4(&m_matWorld), &iNeighborIndex, vLine))
		return true;
	else 
	{
		if (iNeighborIndex != -1)
		{

			while (true)
			{
				if (iNeighborIndex == -1)
					return false;

				
				if (m_vecCell[m_iCurrentCellIndex]->Get_CellType() == CCell::CELLTYPE::TYPE_NORMAL)
				{
					// 노멀 셀에서 점프 셀로 갈 때 처리
					if (m_vecCell[iNeighborIndex]->Get_CellType() == CCell::CELLTYPE::TYPE_JUMP)
					{
						_float3 vPos;
						XMStoreFloat3(&vPos, vPosition);

						// 높이가 크면
						if (m_vecCell[iNeighborIndex]->Is_Height(vPos))
						{
							if (m_vecCell[iNeighborIndex]->IsIn(vPosition, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex, vLine))
							{
								m_iCurrentCellIndex = iNeighborIndex;
								return true;
							}
							else
								return false;
						}else //높이가 낮으면
							return false;
							
					} // 노멀에서 노멀
					else if (m_vecCell[iNeighborIndex]->Get_CellType() == CCell::CELLTYPE::TYPE_NORMAL)
					{
						if (m_vecCell[iNeighborIndex]->IsIn(vPosition, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex, vLine))
						{
							m_iCurrentCellIndex = iNeighborIndex;
							return true;
						}
					}

					// 플레이어의 높이 값이 셀의 높이 값보다 작으면 실패
					/*if (!m_vecCell[iNeighborIndex]->Is_Height(vPos))
						return false;*/
				}
				else
				{	// 점프에서 노멀
					if (m_vecCell[iNeighborIndex]->Get_CellType() == CCell::CELLTYPE::TYPE_NORMAL)
					{
						m_iCurrentCellIndex = iNeighborIndex;
						return true;
					} // 점프에서 점프
					else if (m_vecCell[iNeighborIndex]->Get_CellType() == CCell::CELLTYPE::TYPE_JUMP)
					{
						_float3 vPos;
						XMStoreFloat3(&vPos, vPosition);
						// 높이가 크면
						if (m_vecCell[iNeighborIndex]->Is_Height(vPos))
						{
							if (m_vecCell[iNeighborIndex]->IsIn(vPosition, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex, vLine))
							{
								m_iCurrentCellIndex = iNeighborIndex;
								return true;
							}
							else
								return false;
						}
						else // 높이가 낮으면
							return false;
					}
						

					/*if (m_vecCell[iNeighborIndex]->IsIn(vPosition, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex, vLine))
					{
						m_iCurrentCellIndex = iNeighborIndex;
						return true;
					}*/
				}

				/*if (m_vecCell[iNeighborIndex]->IsIn(vPosition, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex, vLine))
				{
					m_iCurrentCellIndex = iNeighborIndex;
					return true;
				}*/

			}

		}
		else {
			return false;
		}
	}
}

void CNavigation::Find_CurrentCell(_fvector vPosition)
{
	_int iNeighborIndex = -1;
	_float3 vLine = {};

	for (auto& iter : m_vecCell)
	{
		if (iter->IsIn(vPosition, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex,&vLine))
			m_iCurrentCellIndex = iter->Get_Index();
	}
}

_int CNavigation::Find_PositionCell(_fvector vPosition)
{
	_int iNeighborIndex = -1;
	_int iCurrentCellIndex = -1;
	_float3 vLine = {};

	for (auto& iter : m_vecCell)
	{
		if (iter->IsIn(vPosition, XMLoadFloat4x4(&m_matWorld), &iNeighborIndex,&vLine))
			iCurrentCellIndex = iter->Get_Index();
	}

	return iCurrentCellIndex;
}

HRESULT CNavigation::Add_Cell(_float3* pPoints, _uint* iCellIndex, _uint iCellType)
{
	CELL2 Cell = {};
	Cell.vPoints.vVertex0 = pPoints[0];
	Cell.vPoints.vVertex1 = pPoints[1];
	Cell.vPoints.vVertex2 = pPoints[2];

	Cell.iCellType = iCellType;
	
	Cell.iNeighborIndexs.x = -1;
	Cell.iNeighborIndexs.y = -1;
	Cell.iNeighborIndexs.z = -1;

    CCell* pCell = CCell::Create(m_pDevice, m_pContext, Cell,m_vecCell.size(), m_eNaviType);
	if (pCell == nullptr)
		return E_FAIL;

	*iCellIndex = pCell->Get_Index();

	m_vecCell.push_back(pCell);

	if (FAILED(Init_Neighbor_XZ()))
		return E_FAIL;

	return S_OK;
}

void CNavigation::All_Delete_Cell()
{
	for (auto& iter : m_vecCell)
		Safe_Release(iter);
	m_vecCell.clear();

}

void CNavigation::Delete_Cell(_uint iCellIndex)
{
	if (iCellIndex >= m_vecCell.size())
		return;

	Safe_Release(m_vecCell[iCellIndex]);
	m_vecCell.erase(m_vecCell.begin() + iCellIndex);

	for (auto& iter : m_vecCell)
	{
		if (iter->Get_Index() > iCellIndex)
		{
			iter->Set_Index(iter->Get_Index() - 1);

		}

		for (_uint i = 0; i < 3; i++)
		{
			if (iter->Get_NeighborIndex((CCell::LINES)i) == iCellIndex)
				iter->Set_NeighborIndex((CCell::LINES)i, -1);
			else
			{
				_int iIdnx = iter->Get_NeighborIndex((CCell::LINES)i);
				if(iIdnx != -1)
					iter->Set_NeighborIndex((CCell::LINES)i, iIdnx - 1);
			}
		}
	}

	if (FAILED(Init_Neighbor_XZ()))
		return ;
}

void CNavigation::Add_Neighbor(_uint iSourCellIndx, _float3* vSourPoints, _uint iDestCellIndx, _float3* vDestPoints)
{
	CCell::LINES eSourLine, eDestLine;

	eSourLine = m_vecCell[iSourCellIndx]->Get_Line(vSourPoints[0], vSourPoints[1]);
	if (eSourLine == CCell::LINES::LINE_END)
		return;

	eDestLine = m_vecCell[iDestCellIndx]->Get_Line(vDestPoints[0], vDestPoints[1]);
	if (eDestLine == CCell::LINES::LINE_END)
		return;

	_int iSourNeighborIndx(-1), iDestNeighborIndx(-1);

	iSourNeighborIndx = m_vecCell[iSourCellIndx]->Get_NeighborIndex(eSourLine);
	iDestNeighborIndx = m_vecCell[iDestCellIndx]->Get_NeighborIndex(eDestLine);

	if (iSourNeighborIndx == -1 && iDestNeighborIndx == -1)
	{
		m_vecCell[iSourCellIndx]->Set_NeighborIndex(eSourLine, m_vecCell[iDestCellIndx]->Get_Index());
		m_vecCell[iDestCellIndx]->Set_NeighborIndex(eDestLine, m_vecCell[iSourCellIndx]->Get_Index());
	}
}

_bool CNavigation::Compute_MousePos(_uint* iCellIndex)
{
	_float fDist = 0.f;
	_float3 vPos = {};

	for (auto& iter : m_vecCell)
	{
		if(	m_pGameInstance->Intersect(&vPos, &fDist,
			XMLoadFloat3(&iter->Get_Point(CCell::POINT_A)),
			XMLoadFloat3(&iter->Get_Point(CCell::POINT_B)),
			XMLoadFloat3(&iter->Get_Point(CCell::POINT_C)), XMLoadFloat4x4(&m_matWorld)))
		{
			*iCellIndex = iter->Get_Index();
			return true;
		}
	}

	return false;
}

_float CNavigation::Get_Cell_Height(_float3 vPos)
{
	if (m_vecCell[m_iCurrentCellIndex] == nullptr)
		return _float();

	return m_vecCell[m_iCurrentCellIndex]->Get_Height(vPos);
}


HRESULT CNavigation::Save_Navigation(const _char* strFilePath)
{
	
	CMeshData::MESHDATADESC MeshDataDesc = {};

	MeshDataDesc.eModel_Type = CMeshData::MODEL_TYPE::NAVIGATION;
	for (auto& iter : m_vecCell)
	{

		/*CELL		Cell = {};
		Cell.vPoints.vVertex0 = iter->Get_Point(CCell::POINTS::POINT_A);
		Cell.vPoints.vVertex1 = iter->Get_Point(CCell::POINTS::POINT_B);
		Cell.vPoints.vVertex2 = iter->Get_Point(CCell::POINTS::POINT_C);

		Cell.iCellType = (_uint)iter->Get_CellType();

		MeshDataDesc.vecNaviPoints.push_back(Cell);*/
		CELL2		Cell = {};
		Cell.vPoints.vVertex0 = iter->Get_Point(CCell::POINTS::POINT_A);
		Cell.vPoints.vVertex1 = iter->Get_Point(CCell::POINTS::POINT_B);
		Cell.vPoints.vVertex2 = iter->Get_Point(CCell::POINTS::POINT_C);

		Cell.iCellType = (_uint)iter->Get_CellType();

		Cell.iNeighborIndexs.x = iter->Get_NeighborIndex(CCell::LINES::LINE_AB);
		Cell.iNeighborIndexs.y = iter->Get_NeighborIndex(CCell::LINES::LINE_BC);
		Cell.iNeighborIndexs.z = iter->Get_NeighborIndex(CCell::LINES::LINE_CA);

		MeshDataDesc.vecNaviPoints2.push_back(Cell);
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
			if (SourCell == DestCell)
				continue;

			if (DestCell->Compare_Points(SourCell->Get_Point(CCell::POINTS::POINT_A), SourCell->Get_Point(CCell::POINTS::POINT_B)))
			{
				SourCell->Set_NeighborIndex(CCell::LINES::LINE_AB, DestCell->Get_Index());
			}
			if (DestCell->Compare_Points(SourCell->Get_Point(CCell::POINTS::POINT_B), SourCell->Get_Point(CCell::POINTS::POINT_C)))
			{
				SourCell->Set_NeighborIndex(CCell::LINES::LINE_BC, DestCell->Get_Index());
			}
			if (DestCell->Compare_Points(SourCell->Get_Point(CCell::POINTS::POINT_C), SourCell->Get_Point(CCell::POINTS::POINT_A)))
			{
				SourCell->Set_NeighborIndex(CCell::LINES::LINE_CA, DestCell->Get_Index());
			}
		}
	}

	return S_OK;
}

HRESULT CNavigation::Init_Neighbor_XZ()
{
	for (auto& SourCell : m_vecCell)
	{
		for (auto& DestCell : m_vecCell)
		{
			if (SourCell == DestCell)
				continue;

			if (DestCell->Compare_Points_XZ(SourCell->Get_Point(CCell::POINTS::POINT_A), SourCell->Get_Point(CCell::POINTS::POINT_B)))
			{
				SourCell->Set_NeighborIndex(CCell::LINES::LINE_AB, DestCell->Get_Index());
			}
			if (DestCell->Compare_Points_XZ(SourCell->Get_Point(CCell::POINTS::POINT_B), SourCell->Get_Point(CCell::POINTS::POINT_C)))
			{
				SourCell->Set_NeighborIndex(CCell::LINES::LINE_BC, DestCell->Get_Index());
			}
			if (DestCell->Compare_Points_XZ(SourCell->Get_Point(CCell::POINTS::POINT_C), SourCell->Get_Point(CCell::POINTS::POINT_A)))
			{
				SourCell->Set_NeighborIndex(CCell::LINES::LINE_CA, DestCell->Get_Index());
			}
		}
	}

	return S_OK;
}

HRESULT CNavigation::Init_Neighbor_Cell(CCell* pCell)
{


 	for (auto& DestCell : m_vecCell)
	{
		if (pCell == DestCell)
			continue;

		if (DestCell->Compare_Points_XZ(pCell->Get_Point(CCell::POINTS::POINT_A), pCell->Get_Point(CCell::POINTS::POINT_B)))
		{
			pCell->Set_NeighborIndex(CCell::LINES::LINE_AB, DestCell->Get_Index());
		}
		if (DestCell->Compare_Points_XZ(pCell->Get_Point(CCell::POINTS::POINT_B), pCell->Get_Point(CCell::POINTS::POINT_C)))
		{
			pCell->Set_NeighborIndex(CCell::LINES::LINE_BC, DestCell->Get_Index());
		}
		if (DestCell->Compare_Points_XZ(pCell->Get_Point(CCell::POINTS::POINT_C), pCell->Get_Point(CCell::POINTS::POINT_A)))
		{
			pCell->Set_NeighborIndex(CCell::LINES::LINE_CA, DestCell->Get_Index());
		}
	}

	return S_OK;
}

HRESULT CNavigation::File_Load(const _char* strNavigationPath)
{
	
	CMeshData::MESHDATADESC MeshDataDesc;

	CMeshData* pMeshData = CMeshData::Create(MeshDataDesc);

	if (FAILED(pMeshData->Load_Data(strNavigationPath)))
		return E_FAIL;

	if (FAILED(pMeshData->Data_Get(MeshDataDesc)))
		return E_FAIL;

	_uint iSize = MeshDataDesc.vecNaviPoints2.size();
	for (_uint i = 0; i < iSize; i++)
	{
		CCell* pCell = CCell::Create(m_pDevice, m_pContext, MeshDataDesc.vecNaviPoints2[i],m_vecCell.size(), m_eNaviType);
		if (pCell == nullptr)
			return E_FAIL;

		m_vecCell.push_back(pCell);
	}

	Safe_Release(pMeshData);

	/*if (FAILED(Init_Neighbor()))
		return E_FAIL;*/
	/*if (FAILED(Init_Neighbor_XZ()))
		return E_FAIL;*/

	return S_OK;
}

void CNavigation::Set_Cell_Type(_uint iCellType, _uint iIndex)
{
	if (iIndex >= m_vecCell.size() || m_vecCell[iIndex] == nullptr)
		return;

	if (iCellType >= (_uint)CCell::CELLTYPE::TYPE_END)
		return;

	m_vecCell[iIndex]->Set_CellType((CCell::CELLTYPE)iCellType);
}

vector<CCell*> CNavigation::Get_Navigation_Cells()
{
	return m_vecCell;
}

void CNavigation::Update_Buffer(_uint iCellIndex,FLOAT3X3 vPositions)
{
	m_vecCell[iCellIndex]->Update_Buffer(vPositions);
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, NAVITYPE eType, const _char* strNavigationPath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(eType,strNavigationPath))) {
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

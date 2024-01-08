#pragma once
//#include "Base.h"
#include "Navigation.h"

BEGIN(Engine)

class CShader;

class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINTS {POINT_A,POINT_B,POINT_C,POINT_END};
	enum LINES {LINE_AB,LINE_BC,LINE_CA,LINE_END};
	enum CELLTYPE {TYPE_NORMAL,TYPE_JUMP,TYPE_END};

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CCell() = default;

public:
	HRESULT	Initialize(CELL Cell, _uint iIndex, CNavigation::NAVITYPE eType);

public:
	_bool	Compare_Points(_float3 SourPoint, _float3 DestPoint);
	_bool	Compare_Points_XZ(_float3 SourPoint, _float3 DestPoint);
	_bool	IsIn(_fvector vPosition, _fmatrix matWorld, _int* iNeighborIndex, _Out_ _float3* vLine);
	_float	Get_Height(_float3 vPos);
	_bool	Is_Height(_float3 vPos);

public:
	_uint	Get_Index() { return m_iIndex; }
	void	Set_Index(_uint iIndex) { m_iIndex = iIndex; }
	_float3	Get_Point(POINTS eType) {
		return m_pPoints[eType];
	}

	void	Set_NeighborIndex(LINES eType, _int iIndex)
	{
		m_iNeighborIndex[eType] = iIndex;
	}

	CELLTYPE	Get_CellType() { return m_eCell_Type; }

#ifdef _DEBUG
	HRESULT	Render(CShader* pShader, _float4 vColor);

public:
	void	Update_Buffer(FLOAT3X3 vPositions);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

private:
	class CVIBuffer_Cell*	m_pBufferCom = { nullptr };
	class CVIBuffer_DCell*	m_pDBufferCom = { nullptr };

#endif

private:
	_float3		m_pPoints[POINT_END];

	_uint		m_iIndex = { 0 };
	_int		m_iNeighborIndex[LINE_END] = {-1,-1,-1};
	_float3		m_vLineNormal[LINE_END] = {};
	_float3		m_vLine[LINE_END] = {};

private:
	CELLTYPE	m_eCell_Type = { CELLTYPE::TYPE_NORMAL };

private:
	CNavigation::NAVITYPE	m_eNaviType = { CNavigation::NAVITYPE::TYPE_END };

public:
	static	CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,CELL Cell,_uint iIndex,
		CNavigation::NAVITYPE eType);
	virtual	void	Free() override;
};

END
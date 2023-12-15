#pragma once
#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINTS {POINT_A,POINT_B,POINT_C,POINT_END};
	enum LINES {LINE_AB,LINE_BC,LINE_CA,LINE_END};
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CCell() = default;

public:
	HRESULT	Initialize(FLOAT3X3 pPoints);

#ifdef _DEBUG
	HRESULT	Render();

public:
	_bool	Compare_Points(_float3 SourPoint, _float3 DestPoint);
	_bool	IsIn(_fvector vPosition, _fmatrix matWorld, _int* iNeighborIndex);

public:
	_uint	Get_Index() { return m_iIndex; }
	_float3	Get_Point(POINTS eType) { 
		return m_pPoints[eType];
	}

	void	Set_NeighborIndex(LINES eType, _int iIndex)
	{
		m_iNeighborIndex[eType] = iIndex;
	}

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

private:
	class CVIBuffer_Cell*	m_pBufferCom = { nullptr };

#endif

private:
	_float3		m_pPoints[POINT_END];

	_uint		m_iIndex = { 0 };
	_int		m_iNeighborIndex[LINE_END] = {-1,-1,-1};
	_float3		m_vLineNormal[LINE_END] = {};

public:
	static	CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,FLOAT3X3 pPoints );
	virtual	void	Free() override;
};

END
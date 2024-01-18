#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int	iCurrentIndex;

	}NAVIGATION_DESC;

	enum NAVITYPE {TYPE_LOAD,TYPE_DEMO,TYPE_END};

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(NAVITYPE eType,const _char* strNavigationPath);
	virtual	HRESULT	Initialize(void* pArg) override;
#ifdef _DEBUG
	virtual HRESULT	Render() override;
#endif

public:
	void	Update(_float4x4 matWorld);
	_bool	IsMove(_fvector vPosition, _Out_ _float3* vLine);
	void	Find_CurrentCell(_fvector vPosition);
	_int	Find_PositionCell(_fvector vPosition);
	HRESULT	Add_Cell(_float3* pPoints,_uint* iCellIndex, _uint iCellType);
	void	Update_Buffer(_uint iCellIndex, FLOAT3X3 vPositions);
	void	All_Delete_Cell();
	void	Delete_Cell(_uint iCellIndex);
	void	Add_Neighbor(_uint iSourCellIndx,_float3* vSourPoints, _uint iDestCellIndx, _float3* vDestPoints);

public:
	_bool	Is_Alone_Neighbor_Cell(_uint iCellIdnx,_int* iAloneNeighborIndx );
	HRESULT	Init_Neighbor();
	HRESULT	Init_Neighbor_XZ();

public:
	_bool	Compute_MousePos(_uint* iCellIndex);
	_float	Get_Cell_Height(_float3 vPos);
	void	Set_CurrentIndex(_int iCurrentInd) { m_iCurrentCellIndex = iCurrentInd; }
	_int	Get_CurrentIndex() { return m_iCurrentCellIndex; }

public:
	HRESULT	Save_Navigation(const _char * strFilePath);
	HRESULT	File_Load(const _char* strNavigationPath);

public:
	void	Set_Cell_Type(_uint iCellType, _uint iIndex);

public:
	vector<class CCell*> Get_Navigation_Cells();

#ifdef _DEBUG
private:
	class CShader*	m_pShaderCom = { nullptr };
#endif

private:
	vector<class CCell*>	m_vecCell;

private:
	_int					m_iCurrentCellIndex = { -1 };

private:
	static _float4x4		m_matWorld;

	NAVITYPE				m_eNaviType = { NAVITYPE::TYPE_END };

private:
	HRESULT	Init_Neighbor_Cell(CCell* pCell);

public:
	static CNavigation* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, NAVITYPE eType, const _char* strNavigationPath);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	 void		Free() override;
};

END
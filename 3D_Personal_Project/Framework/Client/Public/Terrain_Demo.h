#pragma once
#include "Demo.h"

BEGIN(Engine)

class CVIBuffer_DTerrain;
class CNavigation;
class CCell;

END

BEGIN(Client)

class CMark;

class CTerrain_Demo final : public CDemo
{
public:
	typedef struct tagTerrainDemoValue
	{
		_float	fRadius;
		_bool	bWireFrame;

	}TERRAINDEMOVALUE;

	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK,TYPE_BRUSH, TYPE_END };

private:
	CTerrain_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain_Demo(const CTerrain_Demo& rhs);
	virtual	~CTerrain_Demo() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT	Create_DynamicBuffer(_uint iVerticesXNum = 3, _uint iVerticesZNum = 3);
	HRESULT	Set_Control_Variable(void* pArg);
	_bool	Update_Mouse(_float4* fPickPoint);
	void	Update_HeightMap(_fvector vPickPos, _float fRadius, _float fHeight, _float fSharpness);
	HRESULT	Save_Terrain(const _char* strPath);
	HRESULT	Load_Terrain(const _char* strPath);

public:
	HRESULT	Add_Navigation_Cell(_float3* pPoints, _uint* iCellIndex);
	HRESULT	Save_Navigation(const _char* strPath);
	HRESULT	Load_Navigation(const _char* strPath);
	void	Update_Navigation_Cell(_uint iCellIndex, FLOAT3X3 vPositions);
	void	All_Delete_Cell();
	void	Selected_Delete_Cell(_uint iCellIndex);

	_bool	Picked_Cell(_uint* iCellIndex);

	vector<CCell*>	Get_Navigation_Cells();
	void	Set_Add(_bool bCheck);

private:
	CVIBuffer_DTerrain*		m_pVIBufferCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom[TYPE_END] = { nullptr };
	CNavigation*			m_pNavigationCom = { nullptr };
private:
	_bool	m_bWireFrame = { false };

private:
	_float4	m_vMouseWorldPos = {};
	_float	m_fRadius = { 0.f };
	
private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();
	
public:
	static CTerrain_Demo* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
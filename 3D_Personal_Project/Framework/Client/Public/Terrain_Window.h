#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)

class CCell;

END

BEGIN(Client)

class CTerrain_Window final : public CImGui_Window
{
public:
	enum TERRAINMODE {MODE_HEIGHT,MODE_NAVIGATION,MODE_END};

	typedef	struct tagNaviDemo_Desc
	{
		_float3 vPosition;
		_bool	bCheck = false;
		_uint   iSphereIndex;

	}NAVIDEMO_DESC;

	typedef	struct tagCell_Desc
	{
		
		_uint iCellIndex;
		_uint iSphereIndex[3];

	}NAVI_CELL_DESC;

private:
	CTerrain_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CTerrain_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg) override;
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;
	virtual	HRESULT	Save_Data(const _char* strFilePath) override;
	virtual	HRESULT	Load_Data(const _char* strFilePath) override;

private: /* For. Terrain*/ // 여기서 조정해주는 값
	_int	m_iVertices_Size[2] = {3,3};
	_int	m_iHeight_Control[2] = {};
	_float	m_fSharpness = { 0.f };
	_bool	m_bWireFrame = false;
	_bool	m_bAdd = false;

private: // 외부에서 받아 와야 하는 값
	_float4 m_vPickPos = {};

	//TERRAINDATA	m_Test;
private: /* For. Navigation*/
	TERRAINMODE								m_eCurrentMode = { MODE_END };
	NAVIDEMO_DESC							m_vNaviPos[3] = {};

	vector<NAVI_CELL_DESC>					m_vecCell;

	vector<BoundingSphere*>					m_vecSphere = { nullptr };
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*							m_pEffect = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };

	_uint									m_iCurrentSphereIndex = { 0 };
	_uint									m_iCurrentCellIndex = { 0 };

	_int									m_iCalculate[3] = {-1,-1,-1};
private:
	_int									m_iCurrentNaviModeRadioButton = { 0 };

private:
	_bool									m_bNeviPosTrans = { false };

private:
	void	HeightMap();
	void	Terrain_Update();
	void	Create_HeightMap();	

private:
	void	Navigation();
	void	Navigation_Update();

	void	Calculate_Cell();
	void	Picked_Navigation();

	_bool	Set_NaviPickPos();
	void	Reset_NaviPickPos();
	void	Fix_Navigation();

	void	Sphere_Render();

	void	All_Delete_Cell();
	void	Selected_Delete_Cell();

public:
	static	CTerrain_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual	void	Free() override;
};

END
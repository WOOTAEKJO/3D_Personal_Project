#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CTerrain_Window final : public CImGui_Window
{
public:
	enum TERRAINMODE {MODE_HEIGHT,MODE_NAVIGATION,MODE_END};

	typedef	struct tagNaviDemo_Desc
	{
		_float3 vPosition;
		_bool	bCheck = false;

	}NAVIDEMO_DESC;

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

private: /* For. Terrain*/ // ���⼭ �������ִ� ��
	_int	m_iVertices_Size[2] = {3,3};
	_int	m_iHeight_Control[2] = {};
	_float	m_fSharpness = { 0.f };
	_bool	m_bWireFrame = false;

private: // �ܺο��� �޾� �;� �ϴ� ��
	_float4 m_vPickPos = {};

	//TERRAINDATA	m_Test;
private: /* For. Navigation*/
	TERRAINMODE				m_eCurrentMode = { MODE_END };
	NAVIDEMO_DESC			m_vNaviPos[3] = {};

	vector<BoundingSphere*>					m_vecSphere = { nullptr };
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*							m_pEffect = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };

	_uint					m_iCurrentSphereIndex = { 0 };

private:
	void	HeightMap();
	void	Terrain_Update();
	void	Create_HeightMap();	

private:
	void	Navigation();
	void	Navigation_Update();
	_bool	Set_NaviPickPos();
	void	Reset_NaviPickPos();

	void	Sphere_Render();

public:
	static	CTerrain_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual	void	Free() override;
};

END
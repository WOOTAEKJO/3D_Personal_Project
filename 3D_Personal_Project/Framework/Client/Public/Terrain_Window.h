#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CTerrain_Window final : public CImGui_Window
{

public:
	typedef struct tagTerrain_Window_Desc
	{
	}TERRAINWINDOWDESC;
private:
	CTerrain_Window();
	virtual	~CTerrain_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg) override;
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;

private: /* For. Terrain*/ // 여기서 조정해주는 값
	_int	m_iVertices_Size[2] = {3,3};
	_int	m_iHeight_Control[2] = {};
	_float	m_fSharpness = { 0.f };
	_bool	m_bWireFrame = false;

private: /* For.object*/ // 외부에서 받아 와야 하는 값
	_float4 m_vPickPos = {};

private:
	void	HeightMap();
	void	Terrain_Update();
	void	Create_HeightMap();

public:
	static	CTerrain_Window* Create(void* pArg);
	virtual	void	Free() override;
};

END
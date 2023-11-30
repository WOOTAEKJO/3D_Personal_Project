#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CTerrain_Window final : public CImGui_Window
{
private:
	CTerrain_Window();
	virtual	~CTerrain_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;

private:
	_int	m_iVertices_Size[2] = {3,3};
	_int	m_iHeight_Control[2] = {};
	_float	m_fSharpness = { 0.f };
	_bool	m_bWireFrame = false;

private:
	void	HeightMap();

public:
	static	CTerrain_Window* Create(void* pArg);
	virtual	void	Free() override;
};

END
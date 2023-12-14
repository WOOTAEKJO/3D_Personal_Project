#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CMouse_Window final : public CImGui_Window
{
private:
	CMouse_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CMouse_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg);
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;

private:

public:
	static CMouse_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg);
	virtual	void	Free() override;
};

END
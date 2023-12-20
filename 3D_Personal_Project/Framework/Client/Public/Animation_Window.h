#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CAnimation_Window final : public CImGui_Window
{
private:
	CAnimation_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CAnimation_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg);
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;

private:
	void	Animation();
	void	SubObject();

public:
	static CAnimation_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg);
	virtual	void	Free() override;
};

END
#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CCamera_Window final : public CImGui_Window
{
private:
	CCamera_Window();
	virtual	~CCamera_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg);
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;
	virtual	HRESULT	Save_Data() override;
	virtual	HRESULT	Load_Data() override;

private:

public:
	static CCamera_Window* Create(void* pArg);
	virtual	void	Free() override;
};

END
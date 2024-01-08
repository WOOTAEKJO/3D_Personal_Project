#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CCamera_Window final : public CImGui_Window
{
private:
	CCamera_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CCamera_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg);
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;
	virtual	string	Get_Path() override;
	virtual	HRESULT	Save_Data(const _char* strFilePath) override;
	virtual	HRESULT	Load_Data(const _char* strFilePath) override;

private:

public:
	static CCamera_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg);
	virtual	void	Free() override;
};

END
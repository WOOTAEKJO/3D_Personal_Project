#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CWin_Test final : public CImGui_Window
{
private:
	CWin_Test();
	virtual	~CWin_Test() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;

private:

public:
	static	CWin_Test*	Create(void* pArg);
	virtual	void		Free() override;
};

END
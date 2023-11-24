#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CImGui_Window abstract : public CBase
{
public:
	typedef struct tagImGui_Desc
	{
		wstring	strName;
		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

	}IMGUIDESC;
protected:
	CImGui_Window();
	virtual	~CImGui_Window() = default;

public:
	virtual	HRESULT	Initialize() = 0;
	virtual	void	Tick() = 0;
	virtual	HRESULT	Render() = 0;

protected:

public:
	virtual	void	Free() override;
};

END
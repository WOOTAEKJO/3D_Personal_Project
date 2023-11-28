#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)

class CImGui_Window abstract : public CBase
{
public:
	
protected:
	CImGui_Window();
	virtual	~CImGui_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg);
	virtual	void	Tick() = 0;
	virtual	HRESULT	Render() = 0;

protected:
	CGameInstance*	m_pGameInstance = { nullptr };

protected:
	void*	m_pDesc = { nullptr };

protected:
	void	Begin();
	void	End();

public:
	virtual	void	Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)

class CTerrain_Demo;

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

public:
	virtual	void	Set_Variable(void* pArg) = 0;
	virtual	void	Picked(_float4 vPickPoint) = 0;

protected:
	CGameInstance*	m_pGameInstance = { nullptr };

protected:
	CTerrain_Demo*	m_pTerrain = { nullptr };

protected:
	_float4 m_vPickPos = {};

protected:
	void*	m_pDesc = { nullptr };

protected:
	void	Begin();
	void	End();

public:
	virtual	void	Free() override;
};

END
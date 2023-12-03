#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)

class CTerrain_Demo;
class CDemo;

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
	virtual	void	Terrain_Picked(_float4 vPickPoint) = 0;
	virtual	void	Demo_Picked() = 0;

public:
	void	ImGuizmo(ImGuizmo::MODE eMode, CDemo* pDemo);

protected:
	CGameInstance*	m_pGameInstance = { nullptr };

protected:
	CTerrain_Demo*	m_pTerrain = { nullptr };

protected:
	_float4 m_vPickPos = {};

protected:
	_float	m_fObjectPos[3] = {};
	_float	m_fObjectRot[3] = {};
	_float	m_fObjectScale[3] = { 1.f,1.f,1.f };

	ImGuizmo::OPERATION	m_eOperationType = ImGuizmo::OPERATION::TRANSLATE;
	_float	m_fSnap = { 0.f };
	_float	m_fInterval = { 1.f };
protected:
	void*	m_pDesc = { nullptr };

protected:
	void	Begin();
	void	End();

protected:
	void	Arrow_Button(const string& strTag,_float fInterval,float* fValue);

public:
	virtual	void	Free() override;
};

END
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
	enum TYPE {TYPE_SAVE,TYPE_LOAD,TYPE_END};

	struct  Data
	{
		_int m_iTest = { 1 };
		wstring m_strTest = { L"test" };
		wstring m_strTest2 = { L"test2" };
	};
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
	virtual	HRESULT	Save_Data() = 0;
	virtual	HRESULT	Load_Data() = 0;
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
	void*	m_pDesc = { nullptr }; // 윈도우 정보를 받아오는 변수

protected:
	TCHAR m_szFile[MAX_PATH] = TEXT("");
	OPENFILENAME	m_ofn;

	Data m_Data;
protected:
	void	Begin();
	void	End();

protected: /* For.Json*/
	void	Set_File_Flag(TYPE eType);
	string wstring_To_Json(const wstring& strTag);
	wstring	Json_To_wstring(const string& utf8Str);

protected:
	void	Arrow_Button(const string& strTag,_float fInterval,float* fValue);

public:
	virtual	void	Free() override;
};

END
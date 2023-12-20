#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CImGui_Window;
class CTerrain_Demo;
class CDemo;

class CImGuiMgr final : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)
public:
	enum IMGUIMODE { MODE_STATIC,MODE_TERRAIN,MODE_OBJECT,MODE_CAMERA,MODE_ANIMATION, MODE_END};
	enum WINDOWSTATE {WS_MAIN,WS_FILE,WS_SUB1, WS_SUB2, WS_SUB3, WS_SUB4,WS_END};
	enum FILEMODE { FILEMODE_SAVE, FILEMODE_LOAD,FILEMODE_END};
private:
	CImGuiMgr();
	virtual	~CImGuiMgr() = default;

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void		Tick();
	HRESULT		Render();
	void		Window_Set_Variable(IMGUIMODE eType, WINDOWSTATE eWindowTag, void* pArg);

public: 
	CTerrain_Demo* Get_Terrain() { return m_pTerrain; }

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };

private:
	map<WINDOWSTATE, CImGui_Window*> m_mapWindow[MODE_END];
	IMGUIMODE				M_eCurentMode = MODE_STATIC;

private:
	CTerrain_Demo*		m_pTerrain = { nullptr };

private:
	_float4		m_vPickedPoint = {};

private:
	_bool			m_bGrid = false;
	FILEMODE		m_eFileMode = FILEMODE::FILEMODE_END;

private:
	void		Set_Terrain_Edit();
	void		Set_Object_Edit();
	void		Set_Camera_Edit();
	void		Set_Animation_Edit();

private:
	void		Update_Terrain_Pick();
	void		Update_Demo_Pick();

	HRESULT		Save_Data(const _char* strFilePath);
	HRESULT		Load_Data(const _char* strFilePath);

private:
	void		Grid_Draw();
	void		File_Render();

private:
	HRESULT		Ready_Demo();

private:
	CImGui_Window*	Find_Window(IMGUIMODE eType, WINDOWSTATE eWindowTag);

public:
	virtual	void	Free() override;

};

END
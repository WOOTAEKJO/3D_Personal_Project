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
	enum IMGUIMODE { MODE_STATIC,MODE_TERRAIN,MODE_CAMERA, MODE_END};
private:
	CImGuiMgr();
	virtual	~CImGuiMgr() = default;

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void		Tick();
	HRESULT		Render();
	HRESULT		Add_Demo(const string& strDemoTag, CDemo* pDemo);
	void		Window_Set_Variable(IMGUIMODE eType, void* pArg);

public: /* For. Terrain*/
	HRESULT		Create_HeightMap(_uint iX, _uint iZ);
	HRESULT		Set_Terrain_Variable(void* pArg);
	_float4		Get_PickingMousePoint();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };

private:
	vector<CImGui_Window*>	m_vecWindow[MODE_END];
	IMGUIMODE				M_eCurentMode = MODE_STATIC;

private:
	CTerrain_Demo*		m_pTerrain = { nullptr };
	map<string, CDemo*> m_mapDemo;

private:
	void		Set_Terrain_Edit();
	void		Set_Camera_Edit();

private:
	HRESULT		Ready_Demo();
	CDemo*		Find_Demo(const string& strDemoTag);

public:
	virtual	void	Free() override;

};

END
#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CImGui_Window;
class CTerrain_Demo;

class CImGuiMgr final : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)
public:
	enum STATE { TERRAIN, STATE_END};
private:
	CImGuiMgr();
	virtual	~CImGuiMgr() = default;

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT		Render();

public:
	HRESULT		Create_HeightMap(_uint iX, _uint iZ);
	HRESULT		Delete_HeightMap();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };

private:
	vector<CImGui_Window*>	m_vecWindow;

private:
	CTerrain_Demo* m_pTerrain = { nullptr };

private:
	void		Set_Terrain_Edit();

public:
	virtual	void	Free() override;

};

END
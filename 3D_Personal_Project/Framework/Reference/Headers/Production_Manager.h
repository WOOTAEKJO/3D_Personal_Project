#pragma once
#include "Base.h"

BEGIN(Engine)

class CProduction;

class CProduction_Manager : public CBase
{
private:
	CProduction_Manager();
	virtual	~CProduction_Manager() = default;

public:
	HRESULT	Initialize();
	void	Tick();
	void	Render();

public:
	HRESULT	Add_Production(const wstring& strProductionTag, CProduction* pProduction);
	HRESULT	Add_Actor(const wstring& strProductionTag, const wstring& strActorTag, CGameObject* pActor);

	void	SetUp_Production(const wstring& strProductionTag);
	void	Finish_Production();

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	map<wstring, CProduction*>	m_mapProduction;

private:
	CProduction*	m_pCurrentProduction = { nullptr };

private:
	CProduction* Find_Production(const wstring& strProductionTag);

public:
	static CProduction_Manager* Create();
	virtual	void	Free() override;
};

END
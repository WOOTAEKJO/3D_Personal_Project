#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CGameInstance;

END

BEGIN(Client)

class CGameMgr final : public CBase
{
	DECLARE_SINGLETON(CGameMgr)

public:
	typedef struct tagGmaeMgr_Desc
	{
		wstring		strStartEventName;
		wstring		strEndEventName;

	}GAME_EVENT_DESC;

private:
	CGameMgr();
	virtual ~CGameMgr() = default;

public:
	HRESULT	Initialize();
	void	Tick();

	HRESULT	Add_GameToken(CGameObject* pToken);

public:
	void	Start_Game(GAME_EVENT_DESC Event_Desc);

private:
	CGameInstance* m_pGameInstance = { nullptr };

private:
	list<CGameObject*>	m_listToken;

private:
	_bool	m_bGameActivate = { false };
	GAME_EVENT_DESC m_Event_Desc;

private:
	void	Is_End_Game();

public:
	virtual	void	Free() override;
};

END
#pragma once
#include "Player_State.h"

BEGIN(Client)

class CPlayer_Run final : public CPlayer_State
{
private:
	CPlayer_Run();
	virtual	~CPlayer_Run() = default;

public:
	virtual HRESULT	Initialize(CGameObject * pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

private:
	_bool	m_bCheck = { false };
	_bool	m_bRun = { true };

public:
	static	CPlayer_Run* Create(CGameObject * pGameObject);
	virtual	void			Free();
};

END
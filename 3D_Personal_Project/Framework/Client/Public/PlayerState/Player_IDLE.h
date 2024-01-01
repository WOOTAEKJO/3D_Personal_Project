#pragma once
#include "Player_State.h"

BEGIN(Client)

class CPlayer_IDLE final : public CPlayer_State
{
private:
	CPlayer_IDLE();
	virtual	~CPlayer_IDLE() = default;

public:
	virtual HRESULT	Initialize(CGameObject * pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;



public:
	static	CPlayer_IDLE* Create(CGameObject * pGameObject);
	virtual	void			Free();
};

END
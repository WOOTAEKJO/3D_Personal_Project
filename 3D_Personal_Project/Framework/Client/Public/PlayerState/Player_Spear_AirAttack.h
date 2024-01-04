#pragma once
#include "Player_State.h"

BEGIN(Client)

class CPlayer_Spear_AirAttack final : public CPlayer_State
{
private:
	CPlayer_Spear_AirAttack();
	virtual	~CPlayer_Spear_AirAttack() = default;

public:
	virtual HRESULT	Initialize(CGameObject * pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
private:

public:
	static	CPlayer_Spear_AirAttack* Create(CGameObject * pGameObject);
	virtual	void			Free();
};

END
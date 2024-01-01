#pragma once
#include "Player_State.h"

BEGIN(Client)

class CPlayer_Land final : public CPlayer_State
{
private:
	CPlayer_Land();
	virtual	~CPlayer_Land() = default;

public:
	virtual HRESULT	Initialize(CGameObject * pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;



public:
	static	CPlayer_Land* Create(CGameObject * pGameObject);
	virtual	void			Free();
};

END
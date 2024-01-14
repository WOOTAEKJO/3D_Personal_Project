#pragma once
#include "Player_State.h"

BEGIN(Client)

class CPlayer_Fall final : public CPlayer_State
{
private:
	CPlayer_Fall();
	virtual	~CPlayer_Fall() = default;

public:
	virtual HRESULT	Initialize(CGameObject * pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

public:
	static	CPlayer_Fall* Create(CGameObject * pGameObject);
	virtual	void			Free();
};

END
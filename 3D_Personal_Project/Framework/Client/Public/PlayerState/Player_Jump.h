#pragma once
#include "Player_State.h"

BEGIN(Client)

class CPlayer_Jump final : public CPlayer_State
{
private:
	CPlayer_Jump();
	virtual	~CPlayer_Jump() = default;

public:
	virtual HRESULT	Initialize(CGameObject * pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
private:
	
private:
	void	Jump();
	void	Land();
	_bool	Is_Fall();
	

public:
	static	CPlayer_Jump* Create(CGameObject * pGameObject);
	virtual	void			Free();
};

END
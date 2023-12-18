#pragma once
#include "Client_Defines.h"
#include "State.h"

#include "Player.h"

BEGIN(Client)

class CPlayer_State abstract : public CState
{
public:
	
protected:
	CPlayer_State();
	virtual	~CPlayer_State() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
protected:
	CPlayer*		m_pOwner = { nullptr };
public:
	virtual	void			Free() override;
};

END


#pragma once
#include "Client_Defines.h"
#include "Character_State.h"

#include "Player.h"

BEGIN(Client)

class CPlayer_State abstract : public CCharacter_State
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
	
protected:
	void	Trans_Attack(_bool bCheck);

public:
	virtual	void			Free() override;
};

END


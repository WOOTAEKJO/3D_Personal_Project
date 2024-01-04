#pragma once
#include "Client_Defines.h"
#include "Character_State.h"

#include "NPC.h"

BEGIN(Client)

class CNPC_State abstract : public CCharacter_State
{
public:
	
protected:
	CNPC_State();
	virtual	~CNPC_State() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

protected:
	CNPC*		m_pOwner = { nullptr };

public:
	virtual	void			Free() override;
};

END


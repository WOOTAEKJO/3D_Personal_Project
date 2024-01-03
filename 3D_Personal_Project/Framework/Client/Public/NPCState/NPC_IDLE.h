#pragma once
#include "NPC_State.h"

BEGIN(Client)

class CNPC_IDLE final : public CNPC_State
{
private:
	CNPC_IDLE();
	virtual	~CNPC_IDLE() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

public:
	static CNPC_IDLE* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


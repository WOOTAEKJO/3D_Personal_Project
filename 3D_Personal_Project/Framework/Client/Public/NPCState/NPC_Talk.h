#pragma once
#include "NPC_State.h"

BEGIN(Client)

class CNPC_Talk final : public CNPC_State
{
private:
	CNPC_Talk();
	virtual	~CNPC_Talk() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

public:
	static CNPC_Talk* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


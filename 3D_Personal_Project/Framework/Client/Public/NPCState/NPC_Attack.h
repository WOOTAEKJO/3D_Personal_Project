#pragma once
#include "NPC_State.h"

BEGIN(Client)

class CNPC_Attack final : public CNPC_State
{
private:
	CNPC_Attack();
	virtual	~CNPC_Attack() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

private:
	_bool		m_bFind = { false };

public:
	static CNPC_Attack* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


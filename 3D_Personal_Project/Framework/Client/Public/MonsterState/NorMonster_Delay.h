#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CNorMonster_Delay final : public CMonster_State
{
public:
	
private:
	CNorMonster_Delay();
	virtual	~CNorMonster_Delay() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

private:
	
public:
	static	CNorMonster_Delay* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


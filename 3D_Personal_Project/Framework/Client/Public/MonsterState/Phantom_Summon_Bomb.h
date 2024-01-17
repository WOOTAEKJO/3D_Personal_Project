#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CPhantom_Summon_Bomb final : public CMonster_State
{
public:
	
private:
	CPhantom_Summon_Bomb();
	virtual	~CPhantom_Summon_Bomb() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

public:
	static	CPhantom_Summon_Bomb* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


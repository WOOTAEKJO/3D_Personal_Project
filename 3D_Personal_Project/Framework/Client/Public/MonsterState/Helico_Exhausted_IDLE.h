#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CHelicoScarrow;

class CHelico_Exhausted_IDLE final : public CMonster_State
{
public:
	
private:
	CHelico_Exhausted_IDLE();
	virtual	~CHelico_Exhausted_IDLE() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
	
public:
	static	CHelico_Exhausted_IDLE* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CHelicoScarrow;

class CHelico_Dead final : public CMonster_State
{
public:
	
private:
	CHelico_Dead();
	virtual	~CHelico_Dead() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
	
public:
	static	CHelico_Dead* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


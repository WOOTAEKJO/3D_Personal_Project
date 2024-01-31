#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CPhantom_IntroEnd final : public CMonster_State
{
public:
	
private:
	CPhantom_IntroEnd();
	virtual	~CPhantom_IntroEnd() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

public:
	static	CPhantom_IntroEnd* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


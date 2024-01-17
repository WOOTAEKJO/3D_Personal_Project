#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CPhantom_Bouh final : public CMonster_State
{
public:
	
private:
	CPhantom_Bouh();
	virtual	~CPhantom_Bouh() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

public:
	static	CPhantom_Bouh* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


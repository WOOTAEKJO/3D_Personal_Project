#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CPhantom_Hit final : public CMonster_State
{
public:
	
private:
	CPhantom_Hit();
	virtual	~CPhantom_Hit() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

private:
	_bool	m_bSound = { true };

public:
	static	CPhantom_Hit* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


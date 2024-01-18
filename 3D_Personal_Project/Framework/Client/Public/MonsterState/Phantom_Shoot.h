#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CPhantom_Shoot final : public CMonster_State
{
public:
	
private:
	CPhantom_Shoot();
	virtual	~CPhantom_Shoot() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

private:
	_bool	m_bAttack = { true };

public:
	static	CPhantom_Shoot* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


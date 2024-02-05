#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CPhantom_Dead final : public CMonster_State
{
public:
	
private:
	CPhantom_Dead();
	virtual	~CPhantom_Dead() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

private:
	_bool	m_bParticle = { true };
	_bool	m_bSound = { true };
	_bool	m_bSound2 = { true };

public:
	static	CPhantom_Dead* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


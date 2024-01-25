#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CPhantom_Dash final : public CMonster_State
{
public:
	
private:
	CPhantom_Dash();
	virtual	~CPhantom_Dash() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

private:
	_uint		m_iCount = { 0 };
	_float		m_fChangeTime = { 0.f };

public:
	static	CPhantom_Dash* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


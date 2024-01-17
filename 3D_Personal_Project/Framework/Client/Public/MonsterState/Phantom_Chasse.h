#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CPhantom_Chasse final : public CMonster_State
{
public:
	
private:
	CPhantom_Chasse();
	virtual	~CPhantom_Chasse() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

private:
	_float	m_fTimeAcc = { 0.f };

public:
	static	CPhantom_Chasse* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


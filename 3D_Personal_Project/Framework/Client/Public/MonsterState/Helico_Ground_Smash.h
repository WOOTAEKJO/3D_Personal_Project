#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CHelicoScarrow;

class CHelico_Ground_Smash final : public CMonster_State
{
public:
	
private:
	CHelico_Ground_Smash();
	virtual	~CHelico_Ground_Smash() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
	
private:
	_uint			m_iCount = { 0 };
	_bool			m_bAttack = { true };

public:
	static	CHelico_Ground_Smash* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


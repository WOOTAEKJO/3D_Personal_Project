#pragma once
#include "Monster_State.h"

#include "Monster.h"

BEGIN(Client)

class CHelicoScarrow;

class CHelico_Attack final : public CMonster_State
{
public:
	
private:
	CHelico_Attack();
	virtual	~CHelico_Attack() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
	
private:
	_float			m_fCoolTime = { 0.f };

public:
	static	CHelico_Attack* Create(CGameObject* pGameObject);
	virtual	void			Free() override;
};

END


#pragma once
#include "Client_Defines.h"
#include "Character_State.h"

#include "Monster.h"

BEGIN(Client)

class CMonster_State abstract : public CCharacter_State
{
public:
	
protected:
	CMonster_State();
	virtual	~CMonster_State() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

protected:
	CMonster*		m_pOwner = { nullptr };

protected:
	CMonster::STATE	Dead();

public:
	virtual	void			Free() override;
};

END


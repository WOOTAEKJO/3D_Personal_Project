#pragma once
#include "StateCharge.h"

BEGIN(Client)

class CStateCharge_Test final : public CStateCharge
{
public:
	enum STATE { STATE1, STATE2, STATE_END };
private:
	CStateCharge_Test(CGameObject* pOwner, CStateMachine* pStateMachine);
	virtual	~CStateCharge_Test() = default;

public:
	virtual	HRESULT	Initialize() override;

private:

private:
	HRESULT	Add_State1();
	HRESULT	Add_State2();

public:
	static	CStateCharge_Test* Create(CGameObject* pOwner, CStateMachine* pStateMachine);
	virtual	void	Free() override;
};

END
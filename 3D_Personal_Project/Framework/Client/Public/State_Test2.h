#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_Test2 final : public CState
{
private:
	CState_Test2();
	virtual	~CState_Test2() = default;

public:
	virtual HRESULT	Initialize()override;
	virtual void	State_Enter()override;
	virtual void	State_Priority_Tick(_float fTimeDelta)override;
	virtual void	State_Tick(_float fTimeDelta) override;
	virtual void	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
private:

public:
	static	CState_Test2* Create();
	virtual	void	Free() override;
};

END
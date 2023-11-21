#pragma once
#include "Client_Defines.h"
#include "Node.h"

BEGIN(Client)

class CTestAction final : public CNode
{
private:
	CTestAction();
	virtual	~CTestAction() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:

public:
	static	CTestAction*	Create();
	virtual	void			Free() override;
};

END
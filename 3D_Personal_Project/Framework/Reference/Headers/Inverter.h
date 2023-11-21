#pragma once
#include "Decorator.h"

BEGIN(Engine)

class ENGINE_DLL CInverter final :public CDecorator
{
private:
	CInverter();
	virtual	~CInverter() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:

public:
	static	CInverter*	Create();
	virtual	 void		Free() override;
};

END
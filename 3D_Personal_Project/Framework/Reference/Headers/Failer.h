#pragma once
#include "Decorator.h"

BEGIN(Engine)

class ENGINE_DLL CFailer final : public CDecorator
{
private:
	CFailer();
	virtual	~CFailer() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:

public:
	static	CFailer*	Create();
	virtual	 void		Free() override;
};

END
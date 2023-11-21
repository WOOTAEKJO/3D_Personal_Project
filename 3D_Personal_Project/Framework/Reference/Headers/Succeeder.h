#pragma once
#include "Decorator.h"

BEGIN(Engine)

class ENGINE_DLL CSucceeder final : public CDecorator
{
private:
	CSucceeder();
	virtual ~CSucceeder() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:

public:
	static	CSucceeder*		Create();
	virtual	 void			Free() override;
};

END
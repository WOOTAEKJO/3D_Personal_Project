#pragma once
#include "Composite.h"

BEGIN(Engine)

class ENGINE_DLL CSequence final : public CComposite
{
private:
	CSequence();
	virtual ~CSequence() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:

public:
	static CSequence*	Create();
	virtual	 void		Free() override;
};

END
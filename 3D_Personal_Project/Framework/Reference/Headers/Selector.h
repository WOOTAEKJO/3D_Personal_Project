#pragma once
#include "Composite.h"

BEGIN(Engine)

class ENGINE_DLL CSelector final : public CComposite
{
private:
	CSelector();
	virtual	~CSelector() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:

public:
	static	CSelector* Create();
	virtual	void	Free() override;
};

END
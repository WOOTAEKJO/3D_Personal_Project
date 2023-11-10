#pragma once
#include "Base.h"

BEGIN(Engine)

class CEvent_Function final : public CBase
{
private:
	CEvent_Function();
	virtual	~CEvent_Function() = default;

public:
	HRESULT	Add_Function(function<void()> pFunction);
	void	Execute_Function();

private:
	vector<function<void()>> m_vecFunction;

public:
	static CEvent_Function* Create();
	virtual	void	Free() override;
};

END
#pragma once
#include "Decorator.h"

BEGIN(Engine)

class ENGINE_DLL CRepeator final : public CDecorator
{
private:
	CRepeator();
	CRepeator(_uint iLimit);
	virtual	~CRepeator() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:
	_uint	m_iLimit = { 0 };
	_uint	m_iCount = { 0 };

public:
	static	CRepeator*	Create();
	virtual	void		Free() override;
};

END
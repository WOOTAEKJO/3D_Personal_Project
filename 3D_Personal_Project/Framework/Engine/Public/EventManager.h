#pragma once
#include "Base.h"

BEGIN(Engine)

class CEventManager final : public CBase
{
private:
	CEventManager();
	virtual	~CEventManager() = default;

public:
	HRESULT	Initialize();
	HRESULT	Add_Event(const wstring&, function<void()>);

private:
	map<const wstring, function<void()>> m_mapEvent;

public:
	static CEventManager*	Create();
	virtual	void			Free();
};

END
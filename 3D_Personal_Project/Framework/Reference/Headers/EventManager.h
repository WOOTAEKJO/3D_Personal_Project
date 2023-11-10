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
	HRESULT	Add_Event(const wstring& strEventTag, function<void()> pFunction);
	HRESULT	Execute_Event(const wstring& strEventTag);

private:
	map<const wstring, class CEvent_Function*> m_mapEvent;

private:
	class CEvent_Function* Find_Event(const wstring& strEventTag);

public:
	static CEventManager*	Create();
	virtual	void			Free();
};

END
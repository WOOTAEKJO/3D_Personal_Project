#pragma once
#include "Base.h"

BEGIN(Engine)

class CEvent_Manager final : public CBase
{
private:
	CEvent_Manager();
	virtual	~CEvent_Manager() = default;

public:
	HRESULT	Initialize();
	HRESULT	Add_Event(const wstring& strEventTag, function<void()> pFunction);
	HRESULT	Execute_Event(const wstring& strEventTag);

private:
	map<const wstring, class CEvent_Function*> m_mapEvent;

private:
	class CEvent_Function* Find_Event(const wstring& strEventTag);

public:
	static CEvent_Manager*	Create();
	virtual	void			Free();
};

END
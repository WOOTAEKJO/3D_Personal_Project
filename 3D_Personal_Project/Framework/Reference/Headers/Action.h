#pragma once
#include "Base.h"

BEGIN(Engine)

class CAction final : public CBase
{
private:
	CAction();
	virtual	~CAction() = default;
public:
	HRESULT	Initialize();
	HRESULT	Add_Action(function<void()> pFunction);

	void	Act();

private:
	vector<function<void()>>			m_vecAction;
	
public:
	static	CAction* Create();
	virtual	void	Free() override;
};

END

/*
	- 행동들을 모아둔다.
		- 함수들
*/
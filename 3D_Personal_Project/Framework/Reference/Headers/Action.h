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

private:

public:
	static	CAction* Create();
	virtual	void	Free() override;
};

END

/*
	- ���¿� ���� �ൿ���� ��Ƶд�.

*/
#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState();
	virtual	~CState()=default;
public:
	virtual HRESULT	Initialize();
	virtual void	State_Enter() = 0;
	virtual void	State_Priority_Tick(_float fTimeDelta) = 0;
	virtual void	State_Tick(_float fTimeDelta) = 0;
	virtual void	State_Late_Tick(_float fTimeDelta) = 0;
	virtual void	State_Exit() = 0;

protected:
	
public:
	virtual	void	Free() override;
};

END

/*
	- ��ü���� ���� �ٸ� ���µ��� ������ �ִ�.
	- ��� ���µ��� �θ�� Ŭ����
	- ���´� ENTER, UPDATE, EXIT, CHANGE�� ������ �ִ�.

*/
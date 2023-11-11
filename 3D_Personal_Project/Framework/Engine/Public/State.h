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
	- 객체마다 서로 다른 상태들을 가지고 있다.
	- 모든 상태들의 부모격 클래스
	- 상태는 ENTER, UPDATE, EXIT, CHANGE를 가지고 있다.

*/
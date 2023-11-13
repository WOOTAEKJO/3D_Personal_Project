#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CState final : public CBase
{
public:
	enum STATE { ENTER,PRIORITY_TICK,TICK,LATE_TICK,EXIT,STATE_END };
private:
	CState();
	virtual	~CState()=default;
public:
	virtual HRESULT	Initialize();
	void	State_Enter();
	void	State_Priority_Tick(_float fTimeDelta);
	void	State_Tick(_float fTimeDelta);
	void	State_Late_Tick(_float fTimeDelta);
	void	State_Exit();
public:
	HRESULT	Add_Action(STATE eTickType, function<void()> pFunction);
	HRESULT	Add_Transition(const _uint& iResultStateID, function<bool()> pFunction);
public:
	bool	Is_Transition(_uint* iResultStateID);
private:
	vector<class CAction*> 	m_vecActions;	// state에 따라 구분되는 행동들을 가지고 있다.
	class CTransition*		m_pTransition = { nullptr };

public:
	static	CState* Create();
	virtual	void	Free() override;

};

END

/*
	- 객체마다 서로 다른 상태들을 가지고 있다.
	- 상태는 ENTER, UPDATE, EXIT 를 가지고 있다.
	- 상태 안에서 행동들을 가지고 있다.

*/
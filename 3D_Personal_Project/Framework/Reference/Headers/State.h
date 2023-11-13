#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CStateMachine;

class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState(CStateMachine* pStateMachine);
	virtual	~CState()=default;
public:
	virtual HRESULT	Initialize(CGameObject * pGameObject);
	virtual void	State_Enter()=0;
	virtual void	State_Priority_Tick(_float fTimeDelta) = 0;
	virtual void	State_Tick(_float fTimeDelta) = 0;
	virtual void	State_Late_Tick(_float fTimeDelta) = 0;
	virtual void	State_Exit() = 0;

protected:
	CStateMachine*		m_pStateMachine = { nullptr };

public:
	virtual	void	Free() override;
};

END

/*
	- 객체마다 서로 다른 상태들을 가지고 있다.
	- 상태는 ENTER, UPDATE, EXIT 를 가지고 있다.
	- 상태 안에서 행동들을 가지고 있다.

*/
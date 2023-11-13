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
	- ��ü���� ���� �ٸ� ���µ��� ������ �ִ�.
	- ���´� ENTER, UPDATE, EXIT �� ������ �ִ�.
	- ���� �ȿ��� �ൿ���� ������ �ִ�.

*/
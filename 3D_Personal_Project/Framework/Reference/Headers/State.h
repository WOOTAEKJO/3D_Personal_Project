#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CStateMachine;

class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState();
	virtual	~CState()=default;
public:
	virtual HRESULT	Initialize(CGameObject * pGameObject);
	virtual void	State_Enter()=0;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) = 0;
	virtual _uint	State_Tick(_float fTimeDelta) = 0;
	virtual _uint	State_Late_Tick(_float fTimeDelta) = 0;
	virtual void	State_Exit() = 0;

public:
	void	Set_StateID(const _uint & iStateID) { m_iStateID = iStateID; }

protected:
	_uint				m_iStateID = { 0 };

public:
	virtual	void	Free() override;
};

END

/*
	- ��ü���� ���� �ٸ� ���µ��� ������ �ִ�.
	- ���´� ENTER, UPDATE, EXIT �� ������ �ִ�.
	- ���� �ȿ��� �ൿ���� ������ �ִ�.

*/
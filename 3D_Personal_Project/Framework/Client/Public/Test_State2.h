#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CTest_State2 final : public CState
{
private:
	CTest_State2(CStateMachine* pStateMachine);
	virtual	~CTest_State2() = default;

public:
	virtual HRESULT	Initialize(CGameObject * pGameObject) override;
	virtual void	State_Enter() override;
	virtual void	State_Priority_Tick(_float fTimeDelta) override;
	virtual void	State_Tick(_float fTimeDelta) override;
	virtual void	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
private:
	class CGameObject_Test* m_pOwner = { nullptr };
public:
	static	CTest_State2* Create(CGameObject * pGameObject, CStateMachine* pStateMachine);
	virtual	void			Free();
};

END
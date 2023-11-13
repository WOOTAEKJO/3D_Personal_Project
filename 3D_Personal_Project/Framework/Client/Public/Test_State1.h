#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CTest_State1 final : public CState
{
private:
	CTest_State1(CStateMachine* pStateMachine);
	virtual	~CTest_State1() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual void	State_Priority_Tick(_float fTimeDelta) override;
	virtual void	State_Tick(_float fTimeDelta) override;
	virtual void	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
private:
	class CGameObject_Test*		m_pOwner = { nullptr };
public:
	static	CTest_State1*	Create(CGameObject* pGameObject, CStateMachine* pStateMachine);
	virtual	void			Free();
};

END


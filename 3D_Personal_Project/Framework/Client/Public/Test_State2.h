#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CTest_State2 final : public CState
{
private:
	CTest_State2();
	virtual	~CTest_State2() = default;

public:
	virtual HRESULT	Initialize(CGameObject * pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;
private:
	class CGameObject_Test* m_pOwner = { nullptr };
public:
	static	CTest_State2* Create(CGameObject * pGameObject);
	virtual	void			Free();
};

END
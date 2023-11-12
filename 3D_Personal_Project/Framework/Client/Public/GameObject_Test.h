#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CStateMachine;

END

BEGIN(Client)

class CGameObject_Test final : public CGameObject
{	
private:
	CGameObject_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject_Test(const CGameObject_Test& rhs);
	virtual ~CGameObject_Test() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT	Add_Component();
	HRESULT	Add_Event();
	HRESULT	Init_ETC();

public:
	void	Event_Test1();
	void	Event_Test2();

public:
	bool	Transition_Test1();
	bool	Transition_Test2();

public:
	void	Action_Test1();
	void	Action_Test2();

private:
	CStateMachine* m_pStateMachine = { nullptr };

private:
	class CStateCharge*		m_pStateCharge = { nullptr };	// 상태를 가지고 있는 객체
public:
	/* 원형객체를 생성한다. */
	static CGameObject_Test* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);

	/* 사본객체를 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END
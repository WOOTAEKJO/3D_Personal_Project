#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CStateMachine final : public CComponent
{
public:
	enum STATE {ENTER,PRIORITY_TICK,TICK,LATE_TICK,EXIT,STATE_END};
private:
	CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStateMachine(const CStateMachine& rhs);
	virtual	~CStateMachine()=default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg);
	
public:	
	HRESULT	Add_State(const _uint& iStateID);
	HRESULT	Add_Transition(const _uint& iStateID, const _uint& iResultStateID, function<bool()> pFunction);
	HRESULT	Add_Action(const _uint& iStateID, STATE eStateType, function<void()> pFunction);

	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;

	HRESULT	Init_State(const _uint& iStateID);
	HRESULT	Set_State(const _uint& iStateID);

	bool	Is_Change_State();
	
private:
	class CState*			m_pCurrentState = { nullptr };	// 현재 상태
	_uint					m_iCurrentStateID = { 0 };		// 현재 상태 아이디

private:
	map<const _uint, class CState*>			m_mapState;								// 상태들을 저장
	map<const _uint, class CTransition*>	m_mapTransition;						// 조건들을 저장

private:
	class CState*		Find_State(const _uint& iStateID);
	class CTransition*	Find_Transition(const _uint& iStateID);

public:
	static	CStateMachine* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;

};

END

/*
	- 상태, 행동, 조건을 가지고 있다.
	- 상태는 4가지를 가지고 있다.
		- enter
		- update
			- priority_update
			- update
			- late_update
		- exit
	- 행동은 상태에 따라 다르게 나온다.
	- 조건은 다음 상태로 넘어가는 조건이다.
	- 상태는 행동을 가지고 있다.
*/
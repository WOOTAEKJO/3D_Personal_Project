#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CStateMachine final : public CComponent
{
public:
	enum TICKSTATE {PRIORITY_TICK,TICK,LATE_TICK,TICK_END};
private:
	CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStateMachine(const CStateMachine& rhs);
	virtual	~CStateMachine()=default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg);
	
public:	
	HRESULT	Add_State(const wstring & strStateTag, class CState* pState);
	HRESULT	Add_Transition(const wstring & strStateTag, function<bool()> pFunction, const wstring & strResultStateTag);
	HRESULT	Add_Action(const wstring & strStateTag, TICKSTATE eTickType, function<void()> pFunction);

	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;

	HRESULT	Init_State(const wstring & strStateTag);
	HRESULT	Set_State(const wstring & strStateTag);

	bool	Is_Change_State();
	
private:
	class CState*			m_pCurrentState = { nullptr };	// 현재 상태
	wstring					m_strCurrentStateTag;			// 현재 상태 태그

private:
	map<const wstring, class CState*>		m_mapState;			// 상태들을 저장
	map<const wstring, class CTransition*>	m_mapTransition;	// 조건들을 저장
	map<const wstring, class CAction*>		m_mapAction[TICKSTATE::TICK_END];		// 행동들을 저장

private:
	class CState*		Find_State(const wstring & strStateTag);
	class CTransition*	Find_Transition(const wstring & strStateTag);
	class CAction*		Find_Action(const wstring & strStateTag, TICKSTATE eTickType);

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
		- change
	- 행동은 상태에 따라 다르게 나온다.
	- 조건은 다음 상태로 넘어가는 조건이다.
*/
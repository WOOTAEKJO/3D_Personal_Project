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
	class CState*			m_pCurrentState = { nullptr };	// ���� ����
	_uint					m_iCurrentStateID = { 0 };		// ���� ���� ���̵�

private:
	map<const _uint, class CState*>			m_mapState;								// ���µ��� ����
	map<const _uint, class CTransition*>	m_mapTransition;						// ���ǵ��� ����

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
	- ����, �ൿ, ������ ������ �ִ�.
	- ���´� 4������ ������ �ִ�.
		- enter
		- update
			- priority_update
			- update
			- late_update
		- exit
	- �ൿ�� ���¿� ���� �ٸ��� ���´�.
	- ������ ���� ���·� �Ѿ�� �����̴�.
	- ���´� �ൿ�� ������ �ִ�.
*/
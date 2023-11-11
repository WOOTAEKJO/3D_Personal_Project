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
	class CState*			m_pCurrentState = { nullptr };	// ���� ����
	wstring					m_strCurrentStateTag;			// ���� ���� �±�

private:
	map<const wstring, class CState*>		m_mapState;			// ���µ��� ����
	map<const wstring, class CTransition*>	m_mapTransition;	// ���ǵ��� ����
	map<const wstring, class CAction*>		m_mapAction[TICKSTATE::TICK_END];		// �ൿ���� ����

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
	- ����, �ൿ, ������ ������ �ִ�.
	- ���´� 4������ ������ �ִ�.
		- enter
		- update
			- priority_update
			- update
			- late_update
		- exit
		- change
	- �ൿ�� ���¿� ���� �ٸ��� ���´�.
	- ������ ���� ���·� �Ѿ�� �����̴�.
*/
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CStateMachine final : public CComponent
{
private:
	CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStateMachine(const CStateMachine& rhs);
	virtual	~CStateMachine()=default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg);
	
public:	
	HRESULT	Add_State(const _uint& iStateID,class CState* pState);

	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;

	HRESULT	Init_State(const _uint& iStateID);
	HRESULT	Set_State(const _uint& iStateID);

	void	Is_Change_State(const _uint & iStateID);

private:
	class CState*			m_pCurrentState = { nullptr };	// ���� ����
	_uint					m_iCurrentStateID = { 0 };		// ���� ���� ���̵�

private:
	map<const _uint, class CState*>			m_mapState;		// ���µ��� ����

private:
	class CState*		Find_State(const _uint& iStateID);

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
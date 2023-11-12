#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CStateMachine;

END

BEGIN(Client)

class CStateCharge abstract : public CBase
{
protected:
	CStateCharge(CGameObject* pOwner, CStateMachine* pStateManchine);
	virtual	~CStateCharge() = default;

public:
	virtual	HRESULT	Initialize();

protected:
	CStateMachine*	m_pStateMachine = { nullptr };	// ���� ��ü�κ��� �޾ƿ´�. ���۷��� ���� ���� ��
	CGameObject*	m_pOwner = {nullptr};			// ���� ��ü�� �޾ƿ´�. ���۷��� ���� ���� ��

public:
	virtual	void	Free() override;
};

END


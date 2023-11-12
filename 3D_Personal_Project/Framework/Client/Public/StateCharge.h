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
	CStateMachine*	m_pStateMachine = { nullptr };	// 주인 객체로부터 받아온다. 레퍼런스 관리 해줄 것
	CGameObject*	m_pOwner = {nullptr};			// 주인 객체를 받아온다. 레퍼런스 관리 해줄 것

public:
	virtual	void	Free() override;
};

END


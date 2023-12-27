#pragma once
#include "Client_Defines.h"
#include "State.h"

#include "Monster.h"

BEGIN(Client)

class CMonster_State abstract : public CState
{
public:
	
protected:
	CMonster_State();
	virtual	~CMonster_State() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

protected:
	CMonster*		m_pOwner = { nullptr };
	CModel*			m_pOwnerModel = { nullptr };
	CTransform*		m_pOnwerTransform = { nullptr };
	CNavigation*	m_pOnwerNavigation = { nullptr };

protected:
	_float			m_fTime = { 0.f };
	_bool			m_bAttack = { true };

protected:
	void	Translate(CTransform::STATE eType,_float fSpeed, _float fTimeDelta,_bool bTurn = false);
	void	Is_Attack_Time(_float fTimeDelta,_float fTime);

public:
	virtual	void			Free() override;
};

END


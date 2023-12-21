#pragma once
#include "Client_Defines.h"
#include "State.h"

#include "Player.h"

BEGIN(Client)

class CPlayer_State abstract : public CState
{
public:
	
protected:
	CPlayer_State();
	virtual	~CPlayer_State() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

protected:
	CPlayer*		m_pOwner = { nullptr };
	CModel*			m_pOwnerModel = { nullptr };
	CTransform*		m_pOnwerTransform = { nullptr };
	CNavigation*	m_pOnwerNavigation = { nullptr };

protected:
	_float			m_fTime = { 0.5f };

protected:
	void	Translate(CTransform::STATE eType,_float fSpeed, _float fTimeDelta,_bool bTurn = false);

public:
	virtual	void			Free() override;
};

END


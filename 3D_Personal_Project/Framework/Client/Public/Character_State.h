#pragma once
#include "Client_Defines.h"
#include "State.h"

#include "Player.h"

BEGIN(Client)

class CParticle;

class CCharacter_State abstract : public CState
{
public:
	
protected:
	CCharacter_State();
	virtual	~CCharacter_State() = default;

public:
	virtual HRESULT	Initialize(CGameObject* pGameObject) override;
	virtual void	State_Enter() override;
	virtual _uint	State_Priority_Tick(_float fTimeDelta) override;
	virtual _uint	State_Tick(_float fTimeDelta) override;
	virtual _uint	State_Late_Tick(_float fTimeDelta) override;
	virtual void	State_Exit() override;

protected:
	CModel*			m_pOwnerModel = { nullptr };
	CTransform*		m_pOnwerTransform = { nullptr };
	CNavigation*	m_pOnwerNavigation = { nullptr };
	CRigidBody*		m_pOnwerRigidBody = { nullptr };
	CController*	m_pOnwerController = { nullptr };
	CCollider*		m_pOwnerCollider = { nullptr };

protected:
	_float			m_fTime = { 0.f };
	_bool			m_bAttack = { true };

protected:
	CGameObject* m_pParticle = { nullptr };

protected:
	void	Translate(CTransform::STATE eType,_float fSpeed, _float fTimeDelta,_bool bTurn = false);
	void	Translate_Simple(CTransform::STATE eType, _float fSpeed, _float fTimeDelta, _bool bTurn = false);
	_bool	Is_Attack_Time(_float fTimeDelta, _float fTime, CCollider* pOwnerCol = nullptr);
	void	Reset_Attack_Time(CCollider* pOwnerCol);

protected:
	void	Create_Particle(const wstring& strParticleTag, const wstring& strObjTag,
		CGameObject* pOwner, CGameObject** pOut, _float fLifeTime = 0.f, vector<CBone*>* vecBone = nullptr);
	void	Particle_Loop_SetUp(CGameObject* pParticle,_bool bCheck);

public:
	virtual	void			Free() override;
};

END


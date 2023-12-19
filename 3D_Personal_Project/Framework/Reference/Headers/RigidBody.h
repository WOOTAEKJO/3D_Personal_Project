#pragma once
#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CRigidBody final : public CComponent
{
public:
	enum TYPE {TYPE_VELOCITY,TYPE_ACCEL,TYPE_END};

public:
	typedef struct tagRigidBody_Desc
	{
		CGameObject* pOwner = nullptr;

	}RIGIDBODY_DESC;

private:
	CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRigidBody(const CRigidBody& rhs);
	virtual	~CRigidBody() = default;

public:
	virtual	HRESULT	Initialize_ProtoType() override;
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual void	Priority_Tick(_float fTimeDelta) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	Late_Tick(_float fTimeDelta) override;

public:
	void	Set_GravityPower(_float fPower) { m_fGravity = fPower; }

public:
	void	Jump(_float fJumpPower,_float fGravityPower);
	_bool	Is_Land();
	void	Land();

public:
	void	Force(_fvector vDir, _float fPower,TYPE eType);
	void	Reset_Force(TYPE eType);
	_bool	Is_Power_Zero(TYPE eType);

public:
	CGameObject*	m_pOwner = { nullptr };

private:
	_float			m_fGravity = { -9.8f };

private:
	_bool			m_bGravity = { false };
	_float3			m_vResist = {};
	_float3			m_vPower[TYPE_END] = {};

private:
	void	Update_Transform(TYPE eType, _float fTimeDelta);
	void	Resistance(TYPE eType, _float fTimeDelta);
	void	Near_Zero_Force(TYPE eType);

public:
	static	CRigidBody* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;

};

END
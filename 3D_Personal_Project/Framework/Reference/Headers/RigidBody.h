#pragma once
#include "Component.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CRigidBody final : public CComponent
{
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
	void	Force(_fvector vDir, _float fPower, _float fTimeDelta);

public:
	void	Set_Jump() { m_bJump = true; }

public:
	CGameObject*	m_pOwner = { nullptr };

private:
	_float			m_fGravity = 9.8f;

private:
	_bool			m_bJump = { false };
	_bool			m_bGravity = { false };

	_float3			m_vVelocity = _float3(0.f,0.f,0.f);

	_float3			m_vMaxSpeed;
	_float3			m_vMinSpeed;
	_float3			m_vResist;

public:
	static	CRigidBody* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;

};

END
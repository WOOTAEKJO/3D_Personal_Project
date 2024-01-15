#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CCollider;
class CTexture;
class CRigidBody;

END

BEGIN(Client)

class CCharacter;

class CBullet abstract : public CGameObject
{
public:
	typedef struct tagBullet_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		CCharacter* pOwner = nullptr;
		CCharacter* pTarget = nullptr;
		COLLIDER_LAYER eCollider_Layer;
		_float		fLifeTime = 0.f;
		_float		fSpeed = 0.f;
		_float		fRadius = 1.f;
		_float4		fStartPos;

	}BULLET_DESC;

protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& rhs);
	virtual	~CBullet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CShader*	m_pShaderCom = { nullptr };
	CTexture*	m_pTextureCom = { nullptr };
	CCollider*	m_pColliderCom = { nullptr };
	CRigidBody*		m_pRigidBodyCom = { nullptr };

protected:
	CCharacter*		m_pOwner = { nullptr };

protected:
	_float			m_fTimeAcc = { 0.f };
	_float			m_fLifeTime = { 0.f };
	_float			m_fSpeed = { 0.f };
	_float			m_fRadius = { 1.f };

protected:
	_float3			m_vLook;
	_float4			m_vTargetPos;

protected:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	virtual	CGameObject*	Clone(void* pArg)=0;
	virtual	void	Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CModel;
class CShader;
class CNavigation;
class CCollider;
class CRigidBody;
class CStateMachine;

END

BEGIN(Client)

class CCharacter abstract : public CGameObject
{
public:
	typedef struct tagPhysicsDesc
	{
		_float	fForwardSpeed = 10.f;

		_float	fJumpPower = 10.f;
		_float	fJumpGravity = -9.8f;
		_float  fFallGravity = -30.f;

		_float	fTurnSpeed = 25.f;

		_bool	bGround = true;
		_bool	bJump = false;
		_bool	bDoubleJump = false;
		_bool	bFall = false;
		_bool	bLanding = false;

	}PHYSICS_DESC;

	typedef struct tagStatusDesc
	{
		_int	iMaxHP = 10;
		_int	iCurHP = 10;

		_bool	bHited = false;
		_bool	bDead = false;

	}STATUS_DESC;

protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& rhs);
	virtual	~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	PHYSICS_DESC*	Open_Physics_Desc() { return &m_Physics_Desc; }
	STATUS_DESC*	Open_Status_Desc() { return &m_Status_Desc; }

protected:
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CNavigation*	m_pNavigationCom = { nullptr };
	CCollider*		m_pColliderCom = { nullptr };
	CRigidBody*		m_pRigidBodyCom = { nullptr };
	CStateMachine*	m_pStateMachineCom = { nullptr };

protected:
	wstring		m_strModelTag;
	
	PHYSICS_DESC	m_Physics_Desc;
	STATUS_DESC		m_Status_Desc;

protected:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Component();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
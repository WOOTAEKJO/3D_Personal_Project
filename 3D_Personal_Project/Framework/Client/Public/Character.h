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
	typedef struct tagCharacterDesc : public CGameObject::GAMEOBJECT_DESC
	{
		wstring strModelTag = TEXT("");

	}CHARACTER_DESC;

	typedef struct tagPhysicsDesc
	{
		_float	fForwardSpeed = 1.5f;

		_float	fJumpPower = 2.f;
		_float	fJumpGravity = -9.8f * 0.2f;
		_float  fFallGravity = -30.f * 0.3f;

		_float	fTurnSpeed = 25.f;

		_bool	bJump = false;
		_bool	bDoubleJump = false;

	}PHYSICS_DESC;

	typedef struct tagStatusDesc
	{
		_int	iMaxHP = 10;
		_int	iCurHP = 10;

		_float  fDetection_Range = 0.f;
		_float  fAttack_Range = 0.f;

		_bool	bHited = false; // 맞았는 지를 판단
		_bool	bDead = false; // 죽었는 지를 판단

		_bool  bAttack_able = true; // 공격을 받을 수 있는 지를 판단
		_bool  bTalk = false; // Npc 모드인지를 판단

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

public:
	void		Set_ModelTag(const wstring& strModelTag) { m_strModelTag = strModelTag; }
	wstring		Get_ModelTag() { return m_strModelTag; }

public:
	void		Set_TypeAnimIndex(_uint iAnimTag);

protected:
	CShader*		m_pShaderCom = { nullptr };
	CModel*			m_pModelCom = { nullptr };
	CNavigation*	m_pNavigationCom = { nullptr };
	CCollider*		m_pColliderCom = { nullptr };
	CRigidBody*		m_pRigidBodyCom = { nullptr };
	CStateMachine*	m_pStateMachineCom = { nullptr };

protected:
	wstring			m_strModelTag;
	
	PHYSICS_DESC	m_Physics_Desc;
	STATUS_DESC		m_Status_Desc;

protected:
	map<_uint, _uint>	m_mapTypeAnimIndex;
	_int				m_eCurrentTypeAnimIndex = { -1 };

protected:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Component();
	virtual HRESULT	Ready_Animation();

protected:
	void	Add_TypeAnimIndex(_uint iAnimTag, _uint iAnimIndex);
	_int	Find_TypeAnimIndex(_uint iAnimTag);

protected:
	void	Pushed();
	void	Pushed_Reset();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
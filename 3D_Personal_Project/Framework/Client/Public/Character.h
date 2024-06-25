#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

/*
	���� ĳ���͵��� �θ�� Ŭ�����Դϴ�.
	���� ĳ������ �پ��� ���۰� ���¸� �����մϴ�.
	�߻� Ŭ�����Դϴ�.
*/

BEGIN(Engine)

class CModel;
class CShader;
class CNavigation;
class CCollider;
class CRigidBody;
class CStateMachine;

END

BEGIN(Client)

class CTargetCamera;

class CCharacter abstract : public CGameObject
{
public:
	typedef struct tagCharacterDesc : public CGameObject::GAMEOBJECT_DESC
	{
		wstring strModelTag = TEXT("");

	}CHARACTER_DESC; // ĳ���� ����ü

	typedef struct tagPhysicsDesc
	{
		_float	fForwardSpeed = 1.5f;

		_float	fJumpPower = 2.f;
		_float	fJumpGravity = -9.8f * 0.2f;
		_float  fFallGravity = -30.f * 0.3f;

		_float	fTurnSpeed = 25.f;

		_bool	bJump = false;
		_bool	bDoubleJump = false;

	}PHYSICS_DESC; // ĳ������ ���� Ư�� ����ü

	typedef struct tagStatusDesc
	{
		_int	iMaxHP = 10;
		_int	iCurHP = 10;

		_float  fDetection_Range = 0.f;
		_float  fAttack_Range = 0.f;

		_bool	bHited = false; // �¾Ҵ� ���� �Ǵ�
		_bool	bDead = false; // �׾��� ���� �Ǵ�

		_bool  bAttack_able = true; // ������ ���� �� �ִ� ���� �Ǵ�
		_bool  bTalk = false; // Npc ��������� �Ǵ�

	}STATUS_DESC; // ĳ������ ���� ���� ����ü

protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& rhs);
	virtual	~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype() override; // ���� ��ü�� �ʱ�ȭ
	virtual HRESULT Initialize(void* pArg) override; // �纻 ��ü�� �ʱ�ȭ
	virtual void Priority_Tick(_float fTimeDelta) override; // �켱 ���� ƽ ó��
	virtual void Tick(_float fTimeDelta) override; // �Ϲ����� ƽ ó��
	virtual void Late_Tick(_float fTimeDelta) override; // ���� ƽ ó��
	virtual HRESULT Render() override; // ��ü�� ������
	virtual HRESULT Render_Shadow() override; // ��ü�� �׸��� ������

public: // ĳ������ ���� �� ���� ����
	PHYSICS_DESC*	Open_Physics_Desc() { return &m_Physics_Desc; }
	STATUS_DESC*	Open_Status_Desc() { return &m_Status_Desc; }

public: // �� �±� ����
	void		Set_ModelTag(const wstring& strModelTag) { m_strModelTag = strModelTag; }
	wstring		Get_ModelTag() { return m_strModelTag; }

public: // �ִϸ��̼� ����
	void		Set_TypeAnimIndex(_uint iAnimTag);

public: // ī�޶� ����
	void	Camera_Zoom(_float3 vOffset = _float3(0.f,0.f,0.f));
	void	Camera_Shaking(_float fAmplitude, _float fSpeed, _float fTime);
	void	Camera_SetUp_LookAt_Hegith(_float fHeight = 0.f);
	void	Camera_Target_Change(_float3 vOffset,_float fSensitivity,_bool bCutScene, CGameObject* pTarget);

public: // ����Ʈ ����
	virtual HRESULT	Init_Point_Light();
	void		Set_Light_Activate(_bool bCheck);
	void	Update_Light();

protected: // ĳ������ �⺻ ������Ʈ ����
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
	_float				m_fHitCount = { 0.f };
	_bool				m_bHit_Effect = { false };

protected:
	CLight*				m_pLight = { nullptr };

protected:
	virtual HRESULT Bind_ShaderResources(); // ���̴� ���ҽ� ���ε�
	virtual HRESULT Ready_Component(); // �ʿ��� ������Ʈ �غ�
	virtual HRESULT	Ready_Animation(); // �ִϸ��̼� �غ�
	virtual	HRESULT	Ready_Act(); // ĳ������ ����(�ൿ) �غ�

protected: // �ִϸ��̼� ����
	void	Add_TypeAnimIndex(_uint iAnimTag, _uint iAnimIndex);
	_int	Find_TypeAnimIndex(_uint iAnimTag);

protected:
	void	Pushed(); // �и� ȿ��
	void	Pushed_Reset(); // �и� ȿ�� ����

	void	Reset_Hit(_float fTimeDelta); // ��Ʈ ȿ��
	void	Create_Damage_Effect(_float fLifeTime, const wstring& strTextureTag); // ������ ����Ʈ ����
	void	Create_Soul_Effect(_float fLifeTime); // �ҿ� ����Ʈ ����

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

/*
	게임 캐릭터들의 부모격 클래스입니다.
	게임 캐릭터의 다양한 동작과 상태를 관리합니다.
	추상 클래스입니다.
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

	}CHARACTER_DESC; // 캐릭터 구조체

	typedef struct tagPhysicsDesc
	{
		_float	fForwardSpeed = 1.5f;

		_float	fJumpPower = 2.f;
		_float	fJumpGravity = -9.8f * 0.2f;
		_float  fFallGravity = -30.f * 0.3f;

		_float	fTurnSpeed = 25.f;

		_bool	bJump = false;
		_bool	bDoubleJump = false;

	}PHYSICS_DESC; // 캐릭터의 물리 특성 구조체

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

	}STATUS_DESC; // 캐릭터의 상태 정의 구조체

protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& rhs);
	virtual	~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype() override; // 원형 객체의 초기화
	virtual HRESULT Initialize(void* pArg) override; // 사본 객체의 초기화
	virtual void Priority_Tick(_float fTimeDelta) override; // 우선 순위 틱 처리
	virtual void Tick(_float fTimeDelta) override; // 일반적인 틱 처리
	virtual void Late_Tick(_float fTimeDelta) override; // 늦은 틱 처리
	virtual HRESULT Render() override; // 객체의 렌더링
	virtual HRESULT Render_Shadow() override; // 객체의 그림자 렌더링

public: // 캐릭터의 물리 및 상태 접근
	PHYSICS_DESC*	Open_Physics_Desc() { return &m_Physics_Desc; }
	STATUS_DESC*	Open_Status_Desc() { return &m_Status_Desc; }

public: // 모델 태그 관련
	void		Set_ModelTag(const wstring& strModelTag) { m_strModelTag = strModelTag; }
	wstring		Get_ModelTag() { return m_strModelTag; }

public: // 애니메이션 관련
	void		Set_TypeAnimIndex(_uint iAnimTag);

public: // 카메라 관련
	void	Camera_Zoom(_float3 vOffset = _float3(0.f,0.f,0.f));
	void	Camera_Shaking(_float fAmplitude, _float fSpeed, _float fTime);
	void	Camera_SetUp_LookAt_Hegith(_float fHeight = 0.f);
	void	Camera_Target_Change(_float3 vOffset,_float fSensitivity,_bool bCutScene, CGameObject* pTarget);

public: // 라이트 관련
	virtual HRESULT	Init_Point_Light();
	void		Set_Light_Activate(_bool bCheck);
	void	Update_Light();

protected: // 캐릭터의 기본 컴포넌트 변수
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
	virtual HRESULT Bind_ShaderResources(); // 셰이더 리소스 바인딩
	virtual HRESULT Ready_Component(); // 필요한 컴포넌트 준비
	virtual HRESULT	Ready_Animation(); // 애니메이션 준비
	virtual	HRESULT	Ready_Act(); // 캐릭터의 상태(행동) 준비

protected: // 애니메이션 관련
	void	Add_TypeAnimIndex(_uint iAnimTag, _uint iAnimIndex);
	_int	Find_TypeAnimIndex(_uint iAnimTag);

protected:
	void	Pushed(); // 밀림 효과
	void	Pushed_Reset(); // 밀림 효과 리셋

	void	Reset_Hit(_float fTimeDelta); // 히트 효과
	void	Create_Damage_Effect(_float fLifeTime, const wstring& strTextureTag); // 데미지 이펙트 생성
	void	Create_Soul_Effect(_float fLifeTime); // 소울 이펙트 생성

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
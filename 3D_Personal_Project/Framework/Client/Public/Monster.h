#pragma once
#include "Character.h"

/*
	몬스터들의 부모격 추상 클래스입니다.
	CCharacter 클래스를 상속 받았습니다.
*/

BEGIN(Engine)

class CAICom;

END

BEGIN(Client)

class CMonster abstract : public CCharacter
{
public:
	enum STATE {IDLE, MOVE, ATTACK, DELAY,HITED, DEAD,APPEAR, STATE_END};
	// 몬스터들의 기본 상태 열거체

	enum MONSTER_TYPE {SPOOKETON,SKULLCROSSBOW,MONSTER_TYPE_END};
	// 기본 몬스터들의 타입 열거체

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 디바이스와 디바이스 컨텍스트를 받아 초기화
	CMonster(const CMonster& rhs); // 복사 생성자
	virtual	~CMonster() = default; // 가상 소멸자

public:
	virtual HRESULT Initialize_Prototype() override; // 원형 객체 초기화
	virtual HRESULT Initialize(void* pArg) override; // 사본 객체 초기화
	virtual void Priority_Tick(_float fTimeDelta) override; // 우선순위 틱
	virtual void Tick(_float fTimeDelta) override; // 일반적인 틱
	virtual void Late_Tick(_float fTimeDelta) override; // 늦은 틱
	virtual HRESULT Render() override; // 객체의 렌더
	virtual HRESULT Render_Shadow() override; // 객체 그림자의 렌더

public:
	void	TargetLook(); // Y좌표 고려하지 않고 타겟을 수평으로만 바라본다.
	void	TargetLook_Y(); // Y좌표 고려하여 타겟을 바라본다. 
	_bool	Turn(_float fTimeDelta,_bool bCheck = true); // 회전
	_bool	Is_Target_Range(_float fRange); // 타겟이 범위 내에 있는지 체크

public:
	CCollider* Get_WeaponCollider(); // 무기 콜라이더 반환
	MONSTER_TYPE	Get_MonsterType() { return m_eMonsterType; } // 몬스터 타입 반환
	CTransform* Get_PlayerTransform() { return m_pPlayer_Transform; } // 플레이어의 트랜스폼 컴포넌트 반환

	void		Set_Activate() { m_bActivate = true; } // 몬스터 활성화
	_bool		Is_Activate() { return m_bActivate; } // 몬스터 활성화 체크

	_float4x4	Get_Col_WorldMat(); // 콜라이더의 월드행렬을 가져옴

	void		Start_DeadTime() { m_bDeadTime = true; } // 죽음 타이머 시작

	_bool		Get_DeadTime() { return m_bDeadTime; } // 죽음 타이머 체크

protected:
	CTexture*	m_pNoiseTextureCom = { nullptr }; // 노이즈 텍스쳐 컴포넌트

protected:
	CBone*		m_pSocketBone = { nullptr }; // 소켓 본
	CCollider*	m_pWeaponColliderCom = { nullptr }; // 무기 콜라이더 컴포넌트

protected:
	class CPlayer*	m_pPlayer = { nullptr }; // 플레이어 객체
	CTransform*		m_pPlayer_Transform = { nullptr }; // 플레이어의 트랜스폼 컴포넌트

protected:
	MONSTER_TYPE		m_eMonsterType = { MONSTER_TYPE::MONSTER_TYPE_END };

protected:
	_bool				m_bActivate = { true };
	_bool				m_bDeadTime = { false };

protected: // 디졸브 관련 변수
	_float m_fDissolveAmount = { 0.f };
	_float m_fDissolveGradiationDistance = { 0.f };
	_float3 m_vDissolveGradiationStartColor;
	_float3 m_vDissolveGradiationGoalColor;

protected:
	CGameObject*	m_pLightEffect = { nullptr }; // 라이트 이펙트 객체

protected:
	void	Monster_Dead(_float fTimeDelta); // 몬스터 죽음 처리
	void	Dissolve(_float fAmount, _float fDistance,_float fTimeDelta); // 디졸브 처리

protected:
	virtual HRESULT Bind_ShaderResources() override; // 셰이더 리소스 바인딩
	virtual HRESULT Ready_Component() override; // 컴포넌트 준비
	virtual HRESULT	Ready_State(); // 일반적인 몬스터 상태 준비


public:
	virtual CGameObject* Clone(void* pArg) = 0; // 사본 객체 생성 순수 가상 함수
	virtual	void	Free() override; // 메모리 해제
};

END
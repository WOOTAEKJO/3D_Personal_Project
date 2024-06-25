#pragma once
#include "Monster.h"

/*
	HelicoScarrow 보스 몬스터를 관리하는 클래스입니다.
	CMonster 클래스를 상속받았습니다.
*/

BEGIN(Client)

class CHelicoScarrow final : public CMonster
{
public: // 보스의 상태를 나타내는 열거체입니다.
	enum STATE {DEAD,DIVE,DIVE_IDLE,EXHAUSTED_HIT,EXHAUSTED_IDLE,EXHAUSTED_WAKE,
	HELICO_ATTACK,HELICO_END,HELICO_START,IDLE,DIVE_UP,GROUND_SMASH,STATE_END};

private:
	CHelicoScarrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 디바이스와 디바이스 컨텍스트를 받아 초기화 합니다.
	CHelicoScarrow(const CHelicoScarrow& rhs); // 복사 생성자
	virtual	~CHelicoScarrow() = default; // 가상 소멸자

public:
	virtual HRESULT Initialize_Prototype() override; // 원형 객체 초기화
	virtual HRESULT Initialize(void* pArg) override; // 사본 객체 초기화
	virtual void Priority_Tick(_float fTimeDelta) override; // 우선 순위 틱
	virtual void Tick(_float fTimeDelta) override; // 일반적인 틱
	virtual void Late_Tick(_float fTimeDelta) override; // 늦은 틱
	virtual HRESULT Render() override; // 객체의 렌더링
	virtual HRESULT Render_Shadow() override; // 객체의 그림자 렌더
	virtual HRESULT Render_Blur() override; // 객체의 블러 렌더

public:
	_bool	Is_AllHited(_uint iNum); // 히트 카운트 체크
	void	Reset_Hited(); // 하트 카운트 리셋
	 
	void	Create_Monster(); // 서브 몬스터 생성
	_bool	Is_SubMonster_AllDead(); // 서브 몬스터가 모두 데드인지 판단

	void	Dead_CountDown(); // 보스의 데드 카운트 처리

	void	Creat_Bullet(); // 총알 생성
	void	Create_Shock_Wave(); // 충격파 생성
	void	Shock_Wave_Radius_Compute(); // 충격파 반경 계산

public: // 충돌 발생 시
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	vector<CGameObject*> m_vecSubMonster; // 서브 몬스터 관리 벡터

private:
	_uint		m_iHited_Count[2] = {0,0};
	_bool		m_bSelectBonNum = { true };

private:
	CGameObject* m_pShockWave_Col = { nullptr }; // 충격파 충돌체 객체
	CGameObject* m_pShockWave_Effect = { nullptr }; // 충격파 이펙트 객체

private:
	virtual HRESULT Bind_ShaderResources() override; // 셰이더 리소스 바인딩
	virtual HRESULT Ready_Component() override; // 컴포넌트 준비
	virtual HRESULT	Ready_State() override; // 상태 준비
	virtual HRESULT	Init_Point_Light() override; // 라이트 초기화

private:
	void	Hide(); // 숨는 상태 관련 메서드

public:
	static	CHelicoScarrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 원형 객체 생성
	virtual CGameObject* Clone(void* pArg) override; // 사본 객체 생성
	virtual	void	Free() override; // 메모리 해제
};

END
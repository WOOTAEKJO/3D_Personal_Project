#pragma once
#include "Character.h"

/*
	플레이어의 파츠, 상태를 관리하는 클래스입니다.
	CCharacter 클래스를 상속받았습니다.
*/

BEGIN(Engine)

class CController;

END

BEGIN(Client)

class CPlayer final : public CCharacter
{
public:
	enum STATE { IDLE, RUN, ATTACK1, ATTACK2, ATTACK3, AIR_ATTACK, JUMP, DOUBLEJUMP ,FALL,
		LAND, ROLL, STATE_END };
	// 플레이어 상태 열거체

	enum KEY_STATE {KEY_FRONT, KEY_BACK, KEY_RIGHT, KEY_LEFT, KEY_JUMP, KEY_LB_ATTACK,
		KEY_RB_ATTACK, KEY_ROLL,KEY_STATE_END};
	// 플레이어 키 상태 열거체

	enum WEAPON_TYPE {TYPE_SPEAR, TYPE_SHOVEL,TYPE_END };
	// 웨폰 타입 열거체

	enum PARTS_TYPE {PARTS_BODY,PARTS_WEAPON,PARTS_END};
	// 플레이어 파츠 타입 열거체
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 디바이스와 디바이스 컨텍스트를 받아 초기화
	CPlayer(const CPlayer& rhs); // 복사 생성자
	virtual	~CPlayer() = default; // 가상 소멸자

public:
	virtual HRESULT Initialize_Prototype() override; // 원형 객체 초기화
	virtual HRESULT Initialize(void* pArg) override; // 사본 객체 초기화
	virtual void Priority_Tick(_float fTimeDelta) override; // 우선순위 틱
	virtual void Tick(_float fTimeDelta) override; // 일반적인 틱
	virtual void Late_Tick(_float fTimeDelta) override; // 늦은 틱
	virtual HRESULT Render() override; // 객체 렌더
	
public:
	CGameObject* Find_Parts(PARTS_TYPE ePartsTag); // 파츠를 찾는 메서드
	CModel* Get_BodyModel(); // 몸 파츠 모델 컴포넌트를 반환
	CTransform* Get_BodyTransform(); // 몸 파츠 트랜스폼 컴포넌트를 반환
	CCollider* Get_WeaponCollider(); // 무기 콜라이더 컴포넌트를 반환
	_uint		Get_NextAttackID() { return m_iAttackID; } // 다음 공격 상태 반환
	_int	Get_CurrentState(); // 현재 상태를 반환

public:
	void	Animation_By_Type(STATE eType); // 상태에 맞는 애니메이션으로 전환
	void	Create_Range_Bullet(); // 특정 총알 생성

public: // 충돌 관련 메서드
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	CController*	m_pControllerCom = { nullptr }; // 컨트롤러 컴포넌트

private:
	map<PARTS_TYPE, CGameObject*>	m_mapParts; // 파츠를 관리하는 map 컨테이너

private: // 무기 타입에 맞는 애니메이션 관리
	typedef map<STATE, _uint> ANIMINDEX;
	map<WEAPON_TYPE, ANIMINDEX>	m_mapTypeAnimation;
	WEAPON_TYPE			m_eCurrentWeaponType = { TYPE_END };

private:
	_uint				m_iAttackID = { CPlayer::STATE::ATTACK1 };

private:
	_int* m_pMaxHP = {nullptr};
	_int* m_pCurHP = { nullptr };

private:
	CGameObject* m_pLightEffect = { nullptr };

private:
	virtual HRESULT Bind_ShaderResources() override; // 셰이더 리소스 바인딩
	virtual HRESULT Ready_Component() override; // 컴포넌트 준비
	virtual HRESULT	Init_Point_Light() override; // 포인트 라이트 초기화
	HRESULT	Ready_UI(); // UI 준비

private:
	HRESULT	Ready_State(); // 상태 준비
	HRESULT	Ready_Parts(); // 파츠 준비
	HRESULT	Ready_Controller(); // 컨트롤러 준비
	virtual HRESULT	Ready_Animation() override; // 애니메이션 준비
	virtual	HRESULT	Ready_Act() override; // 행동 준비
	HRESULT	Add_Parts(const wstring& strPrototypeTag, PARTS_TYPE ePartsTag, void* pArg = nullptr); // 파츠 추가
	HRESULT	Add_WeaponType_By_Animation(WEAPON_TYPE eWeaponType, STATE eStateType, _uint iAnimIndex); // 웨폰타입에 따른 애니메이션 추가
	_int Find_AnimIndex(WEAPON_TYPE eWeaponType, STATE eStateType); // 웨폰 타입에 따른 애니메이션 검색

private:
	void			NextAttackID(); // 다음 연계 공격을 준비

public:
	static	CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 원형 객체 생성
	virtual CGameObject* Clone(void* pArg) override; // 사본 객체 생성
	virtual	void	Free() override; // 메모리 해제
};

END
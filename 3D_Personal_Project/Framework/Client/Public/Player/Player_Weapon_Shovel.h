#pragma once
#include "GameObject.h"

/*
	플레이어 무기 삽을 관리하는 클래스입니다.
	CGameObject 클래스를 상속받았습니다.

*/

BEGIN(Engine)

class CTransform;
class CModel;
class CShader;
class CBone;
class CCollider;
class CVIBuffer_Trail;

END

BEGIN(Client)

class CPlayer_Weapon_Shovel final : public CGameObject
{	
public:
	typedef struct tagPlayer_Weapon_Shovel_Desc
	{
		vector<CBone*>		pBones;
		CTransform* pParentsTransform = nullptr;
		CGameObject* pOwner = nullptr;

	}PLAYERSHOVEL_DESC; // 삽 정의 구조체

private:
	CPlayer_Weapon_Shovel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 디바이스와 디바이스 컨텍스트를 받아 초기화
	CPlayer_Weapon_Shovel(const CPlayer_Weapon_Shovel& rhs); // 복사 생성자
	virtual	~CPlayer_Weapon_Shovel() = default;// 가상소멸자

public:
	virtual HRESULT Initialize_Prototype() override; // 원형 객체 초기화
	virtual HRESULT Initialize(void* pArg) override; // 사본 객체 초기화
	virtual void Priority_Tick(_float fTimeDelta) override; // 우선순위 틱
	virtual void Tick(_float fTimeDelta) override; // 일반적인 틱
	virtual void Late_Tick(_float fTimeDelta) override; // 늦은 틱
	virtual HRESULT Render() override; // 객체 렌더
	virtual HRESULT Render_Shadow() override; // 객체 그림자 렌더

public:
	virtual void Load_FromJson(const json& In_Json) override; // 제이슨 로드

public: // 충돌 관련 메서드들
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	CModel*		m_pModelCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };
	CCollider*	m_pColliderCom = { nullptr };

private:
	CBone*		m_pSocketBone = { nullptr };
	_uint		m_iSocketBoneIndex = { 0 };

	CTransform* m_pParentsTransform = { nullptr };
	_float4x4	m_matWorldMat = {}; // 계산 후 셰이더에 바인딩될 월드행렬

private:
	wstring		m_strModelTag;

private:
	CGameObject* m_pTrailEffect = { nullptr }; // 트레일 이펙트
	CPlayer* m_pOwner = { nullptr };
	_bool		m_bTrail = { true };

private:
	HRESULT Bind_ShaderResources(); // 셰이더 리소스 바인딩
	HRESULT Ready_Component(); // 컴포넌트 준비

public:
	static	CPlayer_Weapon_Shovel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 원형 객체 생성
	virtual CGameObject* Clone(void* pArg) override; // 사본 객체 생성
	virtual	void	Free() override; // 메모리 해제
};

END
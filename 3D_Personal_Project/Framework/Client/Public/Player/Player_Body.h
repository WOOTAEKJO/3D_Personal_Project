#pragma once
#include "GameObject.h"

/*
	플레이어의 몸체 파츠를 관리하는 클래스입니다.
	CGameObject 클래스를 상속받았습니다.
*/

BEGIN(Engine)

class CTransform;
class CModel;
class CShader;

END

BEGIN(Client)

class CPlayer_Body final : public CGameObject
{	
public:
	typedef struct tagPlayer_Body_Desc
	{
		CTransform* pParentsTransform = nullptr;

	}PLAYERBODY_DESC; // 플레이어 몸체 구조체
private:
	CPlayer_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 디바이스와 디바이스 컨텍스트를 받아 초기화
	CPlayer_Body(const CPlayer_Body& rhs); // 복사 생성자
	virtual	~CPlayer_Body() = default; // 가상 소멸자

public:
	virtual HRESULT Initialize_Prototype() override; // 원형 객체 초기화
	virtual HRESULT Initialize(void* pArg) override; // 사본 객체 초기화
	virtual void Priority_Tick(_float fTimeDelta) override; // 우선순위 틱
	virtual void Tick(_float fTimeDelta) override; // 일반적인 틱
	virtual void Late_Tick(_float fTimeDelta) override; // 늦은 틱
	virtual HRESULT Render() override; // 객체 렌더
	virtual HRESULT Render_Shadow() override; // 객체 그림자 렌더
	virtual HRESULT Render_Blur() override; // 객체 블러 렌더

public:
	virtual void Write_Json(json& Out_Json) override; // 제이슨 파일 저장
	virtual void Load_FromJson(const json& In_Json) override; // 제이슨 로드

	HRESULT	Load_Data(const _char* strFilePath); // 특정 파일 주소의 제이슨 파일 로드

public:
	void Set_AnimationIndex(_uint iAnimIndex); // 애니메이션 전환

private:
	CModel*		m_pModelCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };

private:
	CTransform* m_pParentsTransform = { nullptr };
	_float4x4	m_matWorldMat = {}; // 계산 후 셰이더에 바인딩될 월드행렬

private:
	HRESULT Bind_ShaderResources(); // 셰이더 리소스 바인딩
	HRESULT Ready_Component(); // 컴포넌트 준비

public:
	static	CPlayer_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // 원형 객체 생성
	virtual CGameObject* Clone(void* pArg) override; // 사본 객체 생성
	virtual	void	Free() override; // 메모리 해제
};

END
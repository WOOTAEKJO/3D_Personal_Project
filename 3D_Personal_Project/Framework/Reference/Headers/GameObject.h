#pragma once

#include "Base.h"
#include "GameInstance.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{ // 클라이언트에서 상속받아야 하기 때문에 클라이언트가 알고 있어야 한다. ENGINE_DLL 사용
public:
	typedef	struct tagGameObjectDesc
	{
		tagGameObjectDesc() {}
		tagGameObjectDesc(_float fSpeed, _float fRotation) :fSpeedPerSec(fSpeed),fRotationPerSec(fRotation) {}

		_float	fSpeedPerSec = 0.f;
		_float	fRotationPerSec = 0.f;

	}GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	// 원본을 생성할 때 디바이스들을 받아오는 생성자
	CGameObject(const CGameObject& rhs);
	// 사본을 생성할 때 원보의 값들을 복사하는 복사 생성자
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype(); // 원본 초기화
	virtual HRESULT Initialize(void* pArg); // 사본 초기화
	virtual void Priority_Tick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	// 업데이트를 먼저, 중간, 나중으로 나누어 사용한다.

	virtual HRESULT Render();
public:
	template<typename T>
	T* Get_Component()
	{
		CComponent* pComponent = Find_Component(TAG_NAME<T>());
		if (pComponent == nullptr)
		{
			MSG_BOX("Failed Find Component");
			return nullptr;
		}

		return dynamic_cast<T*>(pComponent);
	}

public:
	void	Set_WorldMatrix(_float4x4 matWorld);
	_float4x4 Get_WorldMatrix();

public:
	void	Set_Dead() { m_bDead = true; }
	_bool	Get_Dead() { return m_bDead; }

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

protected:
	class CGameInstance*		m_pGameInstance = { nullptr };

protected:
	class CTransform*			m_pTransformCom = { nullptr };
	

protected:
	map<const wstring, class CComponent*>	m_mapComponent;

protected:
	_bool						m_bDead = { false };

protected:
	_bool						m_isCloned = { false };
	// 클론인지 아닌지를 판단

protected:
	HRESULT	Add_Component(_uint iLevelIndex,const wstring& strPrototypeTag,
		const wstring& strComTag, _Inout_ class CComponent** pOut,void* pArg = nullptr);

	template<typename T>
	HRESULT Add_Component(const wstring& strPrototypeTag, T** pCom, void* pArg = nullptr)
	{
		class CComponent* pComponent = Find_Component(TAG_NAME<T>());
		if (pComponent != nullptr)
			return E_FAIL;

		class CComponent* Clone = m_pGameInstance->Add_Component_Clone(m_pGameInstance->Get_Current_Level()
			, strPrototypeTag, pArg);
		if (Clone == nullptr)
			return E_FAIL;

		*pCom = dynamic_cast<T*>(Clone);

		m_mapComponent.emplace(TAG_NAME<T>(), Clone);

		Safe_AddRef(Clone);

		return S_OK;
	}

	class CComponent* Find_Component(const wstring & strComTag);
	HRESULT Delete_Component(const wstring& strComTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

/*
	프로토 타입을 사용하는 이유
		- 보통은 서버에서 데이터들을 관리한다.(또는 파일 입출력)
			-> 데이터들을 클라이언트에서 관리하면 보안에 좋지 않음
		- 여기서 몬스터를 생성한다고 했을 때 서버에서 데이터들을 얻어와서 생성시켜줘야 한다.
		- 하지만 서버에서 데이터들을 얻어오는 과정은 속도가 많이 느리다.
		- 매번 몬스터들을 생성할 때마다 이렇게 해주면 게임이 잠시 멈추는 현상이 발생할 수 있다.
		- 그래서 프로토 타입을 사용하는데
		- 게임을 로딩하는 과정에서 서버에서 데이터를 받아 원본을 만든다.
		- 그리고 필요할 때마다 사본을 만들어 사용하는데, 이 때 이미 서버에서 받은 데이터들이 있기 때문에
			속도 저하가 일어나지 않는다.

	프로토 타입 원본과 사본
		- 원본과 사본이 있기 때문에 생성자, 생성 함수, 초기화 함수를 각각 만들어준다.
			- 원본
				- 디바이스 정보들을 받아오는 생성자 -> default 생성자
				- 원본을 생성하는 create 함수
					-> 생성자, 초기화 함수에서 필요한 값들을 인자로 얻어온다.
				- 원본을 초기화 시키는 Initializer_Prototype 함수
					-> 공통적으로 필요한 것들을 초기화
						예) 서버 또는 파일 입출력에서 받은 데이터
			- 사본
				- 원본에 데이터들을 복사해서 생성하는 복사 생성자
				- 사본을 생성하는 Clone 함수
					-> 인자 값으로 void*형이 들어가 있는데, 이는 어떤 변수라도 받아 올 수 있게 하려는 의도가 있다.
						예) 플레이어의 주소 값, 위치 값들이 저장되어 있는 정수형 배열 등등
				- 사본을 초기화 시키는 Initialize 함수
					-> 공통적으로 필요한 것들이 아닌 사본마다 필요한 것들
						예) 생성되자 마자 플레이어를 쫓는 몬스터
							-> 이럴 때는 플레이어의 정보들이 필요하기 때문에 생성할 때 인자값으로 
								플레이어의 주소를 받아 온다.
*/
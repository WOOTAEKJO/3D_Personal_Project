#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual	~CComponent() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg);

public: /* 임시. 정쌤 방식은 아직 배우지 않은 상태에서 사용*/
	virtual void	Priority_Tick(_float fTimeDelta) {};
	virtual void	Tick(_float fTimeDelta) {};
	virtual void	Late_Tick(_float fTimeDelta) {};

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	class CGameInstance*	m_pGameInstance = { nullptr };

protected:
	bool		m_bClone = { false };

public:
	virtual	CComponent* Clone(void* pArg)=0;
	virtual	void	Free() override;
};

END

/*
	- 컴포넌트들의 최상위 부모격 클래스
	- 프로토 타입을 사용
		- 생성자, 초기화 함수, 생성 함수가 각자 다름
	- 클라이언트가 알아야 하기 때문에 engine_dll 사용
	- 장치 변수들을 받아서 미리 세팅해 놓는다.
		- 자식 컴포넌트들 중에서 사용하는 것들이 많을 것으로 예상
*/
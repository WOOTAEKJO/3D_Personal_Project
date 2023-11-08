#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{  //클라이언트에서 상속받을 수 있게 레벨이 무엇인지 알아야한다. ENGINE_DLL를 사용한 이유
protected:
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize() ;
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	class CGameInstance*	m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END
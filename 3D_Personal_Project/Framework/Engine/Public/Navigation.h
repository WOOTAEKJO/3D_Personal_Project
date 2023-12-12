#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(const _char* strNavigationPath);
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	HRESULT	Render();

#ifdef _DEBUG
private:
	class CShader*	m_pShaderCom = { nullptr };
#endif

private:
	vector<class CCell*>	m_vecCell;

public:
	static CNavigation* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _char* strNavigationPath);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	 void		Free() override;
};

END
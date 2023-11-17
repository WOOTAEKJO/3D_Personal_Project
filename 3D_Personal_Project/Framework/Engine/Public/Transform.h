#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CTransform() = default;

public:
	virtual	HRESULT	Initialize_ProtoType() override;
	virtual	HRESULT	Initialize(void* pArg) override;

private:

public:
	static	CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;

};

END
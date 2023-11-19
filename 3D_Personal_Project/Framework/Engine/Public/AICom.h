#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CAICom final : public CComponent
{
private:
	CAICom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAICom(const CAICom& rhs);
	virtual	~CAICom() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg);

public:
	virtual void	Priority_Tick(_float fTimeDelta) {};
	virtual void	Tick(_float fTimeDelta) {};
	virtual void	Late_Tick(_float fTimeDelta) {};

private:

public:
	static	CAICom*		Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void		Free() override;
};

END
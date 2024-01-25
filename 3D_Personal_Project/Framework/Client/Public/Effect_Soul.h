#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Soul final : public CEffect
{
private:
	CEffect_Soul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Soul(const CEffect_Soul& rhs);
	virtual	~CEffect_Soul() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

private:
	void	Upward(_float fTimeDelta);
	
private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

public:
	static CEffect_Soul* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
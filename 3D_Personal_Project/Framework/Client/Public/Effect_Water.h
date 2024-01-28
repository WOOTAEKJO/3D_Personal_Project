#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Water final : public CEffect
{
private:
	CEffect_Water(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Water(const CEffect_Water& rhs);
	virtual	~CEffect_Water() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

private:
	_float	m_fTimeDelta = { 0.f };

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	

public:
	static CEffect_Water* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
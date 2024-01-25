#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Damage final : public CEffect
{
private:
	CEffect_Damage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Damage(const CEffect_Damage& rhs);
	virtual	~CEffect_Damage() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

private:
	
private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	

public:
	static CEffect_Damage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
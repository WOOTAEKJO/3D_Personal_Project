#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Halo final : public CEffect
{
private:
	CEffect_Halo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Halo(const CEffect_Halo& rhs);
	virtual	~CEffect_Halo() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CTexture* m_pMaskTexture = { nullptr };
private:
	
private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

public:
	static CEffect_Halo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
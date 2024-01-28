#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Target final : public CEffect
{
public:
	typedef struct tagEffect_TargetInfo : public CEffect::EFFECTINFO
	{
		_float4 vColor = { 1.f,1.f,1.f,1.f };
		_float  fRadius = { 0.f };
		_float  fCompareRadius = { 0.f };

	}EFFECT_TARGETINFO;

private:
	CEffect_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Target(const CEffect_Target& rhs);
	virtual	~CEffect_Target() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_Radius(_float fRadius) { m_fRadius = fRadius; }

private:
	
private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	_float		m_fRadius = { 0.f };
	_float		m_fCompareRadius = { 0.f };

public:
	static CEffect_Target* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
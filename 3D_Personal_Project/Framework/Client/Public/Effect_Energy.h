#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Energy final : public CEffect
{
public:
	typedef struct tagEffect_EnergyInfo : public CEffect::EFFECTINFO
	{
		_float4 vColor = { 1.f,1.f,1.f,1.f };

	}EFFECT_ENERGYINFO;
private:
	CEffect_Energy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Energy(const CEffect_Energy& rhs);
	virtual	~CEffect_Energy() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	void	Update_Pos();

private:
	

public:
	static CEffect_Energy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_DashRoad final : public CEffect
{
public:
	typedef struct tagEffect_DashRoadInfo : public CEffect::EFFECTINFO
	{
		_float4 vColor = { 1.f,1.f,1.f,1.f };

	}EFFECT_DASHROADINFO;

private:
	CEffect_DashRoad(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_DashRoad(const CEffect_DashRoad& rhs);
	virtual	~CEffect_DashRoad() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

	void	SetUp();

private:
	_float* m_pSpare = {nullptr};

public:
	static CEffect_DashRoad* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
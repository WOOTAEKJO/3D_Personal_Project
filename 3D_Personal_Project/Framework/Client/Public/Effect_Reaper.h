#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Reaper final : public CEffect
{
public:
	enum COMPUTE_TYPE {POSX, POSY, POSZ, POS_END};

	typedef struct tagEffect_ReaperInfo : public CEffect::EFFECTINFO
	{
		_float4 vColor = { 1.f,1.f,1.f,1.f };
		COMPUTE_TYPE eComputeType = { POS_END };

	}EFFECT_REAPERINFO;

private:
	CEffect_Reaper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Reaper(const CEffect_Reaper& rhs);
	virtual	~CEffect_Reaper() = default;
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
	void	Compute_Pos();
	
private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	CTransform* m_pOwnerTransform = { nullptr };

private:
	_float		m_fRadius = { 0.f };
	_float		m_fMaxRadius = { 0.f };
	_float		m_fTotal_Distance = { 0.f };
	COMPUTE_TYPE m_eComputeType = { COMPUTE_TYPE::POS_END };

public:
	static CEffect_Reaper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
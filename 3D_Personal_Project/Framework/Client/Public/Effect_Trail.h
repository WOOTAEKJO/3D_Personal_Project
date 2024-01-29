#pragma once
#include "Effect.h"

BEGIN(Engine)


END

BEGIN(Client)

class CEffect_Trail final : public CEffect
{
public:
	typedef struct tagEffect_TrailInfo : public CEffect::EFFECTINFO
	{
		_float3 vTrailPos_0;
		_float3 vTrailPos_1;
		_uint iTrailMaxCnt;
		_uint iLerpPointNum;
		
		_float4 vSolid_Color;

	}EFFECT_TRAILINFO;
private:
	CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Trail(const CEffect_Trail& rhs);
	virtual	~CEffect_Trail() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Trail_Update(_fmatrix matOwner);
	void	Trail_Reset(_fmatrix matOwner);

private:
	CVIBuffer_Trail* m_pTrailCom = { nullptr };

private:
	EFFECT_TRAILINFO m_TrainInfo = {};

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

public:
	static CEffect_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
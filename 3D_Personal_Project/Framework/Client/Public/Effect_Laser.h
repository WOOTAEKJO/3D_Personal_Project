#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Laser final : public CEffect
{
public:
	typedef struct tagEffect_LaserInfo : public CEffect::EFFECTINFO
	{
		_float4 vColor = { 1.f,1.f,1.f,1.f };
		CGameObject* pTarger = nullptr;

	}EFFECT_LASERINFO;

private:
	CEffect_Laser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Laser(const CEffect_Laser& rhs);
	virtual	~CEffect_Laser() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

private:
	CTransform* m_pTargetTransform = { nullptr };

private:
	_float3	m_vSize = {1.f,1.f,1.f};
	_float4x4 m_matWorld;
	
private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	void	ReSize();
	void	Update_World();

public:
	static CEffect_Laser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
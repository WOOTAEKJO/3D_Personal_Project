#pragma once
#include "Particle.h"

BEGIN(Client)

class CParticle_Sub final : public CParticle
{
private:
	CParticle_Sub(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Sub(const CParticle_Sub& rhs);
	virtual	~CParticle_Sub() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Load_FromJson(const json& In_Json) override;

private:
	_float	m_fTime = { 0.f };

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CParticle_Sub* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject* Clone(void* pArg);
	virtual	void	Free() override;
};

END
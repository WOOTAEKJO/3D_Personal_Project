#pragma once
#include "Particle.h"

BEGIN(Client)

class CParticle_Normal final : public CParticle
{
private:
	CParticle_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Normal(const CParticle_Normal& rhs);
	virtual	~CParticle_Normal() = default;
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

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CParticle_Normal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject* Clone(void* pArg);
	virtual	void	Free() override;
};

END
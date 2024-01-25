#pragma once
#include "Particle.h"

BEGIN(Client)

class CParticle_Sprite final : public CParticle
{
private:
	CParticle_Sprite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Sprite(const CParticle_Sprite& rhs);
	virtual	~CParticle_Sprite() = default;
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
	_float		m_fFrame = { 0.f };
	_float		m_fMaxFrame = { 0.f };
	
private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CParticle_Sprite* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject* Clone(void* pArg);
	virtual	void	Free() override;
};

END
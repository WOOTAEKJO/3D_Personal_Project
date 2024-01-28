#pragma once
#include "Particle.h"

BEGIN(Client)

class CParticle_Attack final : public CParticle
{
public:
	typedef struct tagAttackParticle : public CParticle::PARTICLEINFO
	{
		_float4	vPos;
		_float3	vDir;
		_bool  bKeep = false;

	}ATTACKPARTICLE_DESC;

private:
	CParticle_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Attack(const CParticle_Attack& rhs);
	virtual	~CParticle_Attack() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Load_FromJson(const json& In_Json) override;

public:
	void	Set_Pos(_float4 vPos) { m_vPos = vPos; }

private:
	_float	m_fTime = { 0.f };

private:
	_float4	m_vPos = {0.f,0.f,0.f,1.f};
	_float3	m_vDir = {};
	_bool	m_bKeep = true;

private:
	_float		m_fFrame = { 0.f };
	_float		m_fMaxFrame = { 0.f };
	_bool		m_bSprite = { false };

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CParticle_Attack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject* Clone(void* pArg);
	virtual	void	Free() override;
};

END
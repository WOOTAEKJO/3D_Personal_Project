#pragma once
#include "Particle.h"

BEGIN(Client)

class CParticle_Stage final : public CParticle
{
public:
	typedef struct tagStageParticle : public CParticle::PARTICLEINFO
	{
		_float4	vPos;

	}STAGEPARTICLE_DESC;

private:
	CParticle_Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Stage(const CParticle_Stage& rhs);
	virtual	~CParticle_Stage() = default;
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
	_float4 m_vPos;

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CParticle_Stage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject* Clone(void* pArg);
	virtual	void	Free() override;
};

END
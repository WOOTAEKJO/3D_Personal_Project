#pragma once
#include "Demo.h"

BEGIN(Engine)

class CVIBuffer_Particle_Point;

END

BEGIN(Client)

class CParticle_Demo final : public CDemo
{
public:
	enum COLORTYPE { TEXTURE_COLOR, SOLID_COLOR, TYPE_END };

	typedef struct tagParticleObject_Demo_Desc : public DEMO_DESC
	{
		_float3		vCenter;
		_float		fRange;
		_float2		fSpeed;
		_float2		fScale;
		_float2		fLifeTime;
		_uint		iInstanceNum;

		_float4		vColor;
		COLORTYPE	eTextureType;

		_float3		vDir;
		_float3		vRotation;

		_float3		vRunDir;

	}PARTICLEDEMO_DESC;

private:
	CParticle_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Demo(const CParticle_Demo& rhs);
	virtual	~CParticle_Demo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Set_Control_Variable(void* pArg) override;

public:
	_bool	Get_Picked();

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

public:
	void	Set_Update(_bool bCheck) { m_bUpdate = bCheck; }
	void	TextureType(COLORTYPE eType);

private:
	CShader*		m_pShaderCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CVIBuffer_Particle_Point* m_pBufferCom = { nullptr };

private:
	_bool			m_bUpdate = { false };

private:
	PARTICLEDEMO_DESC	m_eParticleInfo;
	
private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;

public:
	static CParticle_Demo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
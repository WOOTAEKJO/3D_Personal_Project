#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CParticle_Demo;

class CEffect_Window final : public CImGui_Window
{
public:
	typedef struct tagParticle_Info
	{
		_float3		vCenter = _float3(0.f,0.f,0.f);
		_float		fRange = 1.f;
		_float2		fSpeed = _float2(0.f,0.f);
		_float2		fScale = _float2(1.f,1.f);
		_float2		fLifeTime = _float2(1.f,1.f);
		_int		iInstanceNum = { 1 };
		_float4		vColor = _float4(1.f, 1.f, 1.f, 1.f);
		_int		iTextureType = { 0 };

		_float3		vDir = _float3(1.f,0.f,0.f);
		_float3		vRotation = _float3(0.f, 0.f, 0.f);

		_float3		vRunDir = _float3(0.f,0.f,0.f); // Runtime 방향
		_float2		vRunRotation = _float2(0.f, 0.f); // 실시간 회전 각도;

	}PARTICLE_INFO;

private:
	CEffect_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CEffect_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg);
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;
	virtual	string	Get_Path() override;

private:
	vector<wstring>					m_vecTextureTag;
	wstring							m_strPickTextureTag;

private:
	CParticle_Demo*		m_pParticle = { nullptr };
	PARTICLE_INFO		m_eParticleInfo;

private:
	_bool				m_bUpdate = { false };
	
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

	_bool			m_bRandomRotation[3] = { false,false,false };

private:
	void	Particle();
	void	Create_Particle();
	void	Particle_Rotation();

public:
	static CEffect_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg);
	virtual	void	Free() override;
};

END
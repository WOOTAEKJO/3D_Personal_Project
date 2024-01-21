#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CParticle_Demo;

class CEffect_Window final : public CImGui_Window
{
public:
	//typedef struct tagParticle_Info
	//{
	//	_float3		vCenter = _float3(0.f, 0.f, 0.f);

	//	_float		fRange = 1.f;

	//	_float2		fSpeed = _float2(0.f, 0.f);

	//	_float2		fScale = _float2(1.f, 1.f);
	//	_float		fScaleControl = {0.1f};

	//	_float2		fLifeTime = _float2(1.f, 1.f);

	//	_float4		vColor = _float4(1.f, 1.f, 1.f, 1.f);

	//	_float3		vDir = _float3(1.f, 0.f, 0.f);// 생성할 때 사용하는 방향
	//	_float3		vRunDir = _float3(0.f, 0.f, 0.f); // 실시간 방향

	//	_float2		fRotation[3] = {}; // 생성 각도 최소 최댓값 xyz
	//	_float2		fRunRotation[3] = {}; // 실시간 회전 최소 최댓값 xyz

	//	_int		iTextureType = { 0 };// 텍스쳐 타입

	//	_int		iInstanceNum = { 1 }; // 인스턴스 갯수

	//	_bool		bLoop = {false}; // 무한 루프

	//}PARTICLE_INFO;

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

public:
	virtual	HRESULT	Save_Data(const _char* strFilePath);
	virtual	HRESULT	Load_Data(const _char* strFilePath);

private:
	vector<wstring>					m_vecTextureTag;
	wstring							m_strPickTextureTag;

private:
	CParticle_Demo*		m_pParticle = { nullptr };
	INSTANCING_DESC		m_eParticleInfo = {};

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
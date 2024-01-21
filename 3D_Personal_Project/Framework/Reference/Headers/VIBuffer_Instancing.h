#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	enum INSTANCING_TYPE {TYPE_PARTICLE, TYPE_MESH,TYPE_END};

public:
	typedef struct tagInstancing_Desc
	{
		_float3		vCenter;
		_float		fRange;
		_float2		fSpeed;
		_float2		fScale;
		_float2		fLifeTime;

		_float4		vColor;

		_float3		vDir; // 생성할 때 사용하는 방향
		_float3		vRotation;

		_float3		vRunDir; // 실시간 방향
		_float2		vRunRotation; // 실시간 회전 각도 최소 최댓 값
		
	}INSTANCING_DESC;

protected:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs);
	virtual	~CVIBuffer_Instancing() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual HRESULT	Bind_Buffer() override;
	virtual	void	Update(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	vector<_float4x4>	Get_InstanceVertex() { return m_vecInstanceVertex; }

protected:
	ID3D11Buffer*	m_pInstanceBuffer = { nullptr };
	_uint			m_iInstanceStride = { 0 };
	_uint			m_iInstanceNum = { 0 };
	_uint			m_iIndexCountPerInstance = { 0 };

protected:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

protected:
	_float*			m_pSpeeds = { nullptr };
	_float*			m_pLifeTime = { nullptr };
	_float*			m_pTimeAcc = { nullptr };
	_float*			m_pRunRotation = { nullptr };
	_float*			m_pScale = { nullptr };
	_float4*		m_pPos = { nullptr };

	INSTANCING_DESC	m_Instancing_Desc;
	//_float			m_fTimeAcc = { 0.f };

protected:
	vector<_float4x4>		m_vecInstanceVertex;

protected:
	INSTANCING_TYPE	m_eInstanceType = { TYPE_END };

protected:
	HRESULT		Init_Particle(VTXINSTANCING* pVerpostex);
	HRESULT		Init_Mesh(VTXINSTANCING* pVerpostex);

	HRESULT		Init_InstanceBuffer();

protected:
	_vector		CenterToPos(_float4 vPos);

protected:
	void		Reset();

public:
	virtual	CComponent* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CVIBuffer_Instancing abstract : public CVIBuffer
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
	INSTANCING_DESC	m_Instancing_Desc;
	_float			m_fTimeAcc = { 0.f };

protected:
	vector<_float4x4>		m_vecInstanceVertex;

protected:
	INSTANCING_TYPE	m_eInstanceType = { TYPE_END };

private:
	HRESULT		Init_Particle(VTXINSTANCING* pVerpostex);
	HRESULT		Init_Mesh(VTXINSTANCING* pVerpostex);

public:
	virtual	CComponent* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
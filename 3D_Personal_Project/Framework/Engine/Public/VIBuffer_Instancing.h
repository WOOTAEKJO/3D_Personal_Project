#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	enum INSTANCING_TYPE {TYPE_PARTICLE, TYPE_MESH,TYPE_END};

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
	INSTANCING_DESC* Open_InstancingDesc() { return &m_Instancing_Desc; }

protected:
	ID3D11Buffer*	m_pInstanceBuffer = { nullptr };
	_uint			m_iInstanceStride = { 0 };
	_uint			m_iInstanceNum = { 0 };
	_uint			m_iIndexCountPerInstance = { 0 };

protected:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

protected:
	_float3*			m_pSpeeds = { nullptr };
	_float3*			m_pSpeedAcc = { nullptr };

	_float*			m_pLifeTime = { nullptr };
	_float*			m_pTimeAcc = { nullptr };
	
	_float*			m_pScale = { nullptr };
	_float*			m_pScaleTimeAcc = { nullptr };

	_float4*		m_pPos = { nullptr };

	_float3*		m_pRotation = { nullptr };
	_float3*		m_pRunRotation = { nullptr };
	_float3*		m_pRunRotationAcc = { nullptr };

	_float3*		m_pDir = { nullptr };

	INSTANCING_DESC	m_Instancing_Desc;

protected:
	vector<_float4x4>		m_vecInstanceVertex;

protected:
	INSTANCING_TYPE		m_eInstanceType = { TYPE_END };

protected:
	_bool		m_bLoad = { false };

protected:
	_bool		m_bFinish = { false };

protected:
	HRESULT		Init_Particle(VTXINSTANCING* pVerpostex);
	HRESULT		Init_Mesh(VTXINSTANCING* pVerpostex);

	HRESULT		Init_InstanceBuffer();

protected:
	_vector		CenterToPos(_float4 vPos);

protected:
	void		Reset(VTXINSTANCING* pInstancing,_uint iIndx);

public:
	virtual	CComponent* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
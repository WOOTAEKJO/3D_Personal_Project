#pragma once
#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle_Point final : public CVIBuffer_Instancing
{
public:
	typedef struct tagParticle_Point_Desc : public CVIBuffer_Instancing::INSTANCING_DESC
	{
		_uint	iInstanceNum;

	}INSTANCING_PARTICLEPOINT_DESC;

private:
	CVIBuffer_Particle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle_Point(const CVIBuffer_Particle_Point& rhs);
	virtual	~CVIBuffer_Particle_Point() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg) override;

private:
	INSTANCING_PARTICLEPOINT_DESC	m_ePointDesc;

private:
	HRESULT	Init_Buffer();
	
public:
	static CVIBuffer_Particle_Point* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
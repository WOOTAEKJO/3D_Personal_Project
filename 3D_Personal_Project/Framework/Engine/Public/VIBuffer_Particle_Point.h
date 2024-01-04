#pragma once
#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle_Point final : public CVIBuffer_Instancing
{
private:
	CVIBuffer_Particle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle_Point(const CVIBuffer_Particle_Point& rhs);
	virtual	~CVIBuffer_Particle_Point() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(_uint iInstanceNum);
	virtual	HRESULT	Initialize(void* pArg) override;

private:
	
public:
	static CVIBuffer_Particle_Point* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iInstanceNum);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
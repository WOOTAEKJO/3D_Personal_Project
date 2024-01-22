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
	virtual	HRESULT	Initialize_ProtoType(const _char * strFilePath);
	virtual	HRESULT	Initialize(void* pArg) override;

public:
	HRESULT	Save_Particle(const _char * strFilePath);

private:
	HRESULT	Init_Buffer();
	
public:
	static CVIBuffer_Particle_Point* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,const _char* strFilePath);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
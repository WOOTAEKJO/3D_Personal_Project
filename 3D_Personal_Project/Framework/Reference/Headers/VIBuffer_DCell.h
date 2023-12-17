#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_DCell final : public CVIBuffer
{
private:
	CVIBuffer_DCell(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	CVIBuffer_DCell(const CVIBuffer_DCell& rhs);
	virtual	~CVIBuffer_DCell() = default;
public:
	virtual	HRESULT	Initialize_ProtoType(const _float3 * pPoints);
	virtual	HRESULT	Initialize(void* pArg) override;

public:
	void	Update_Buffer(FLOAT3X3 vPositions);

private:

public:
	static	CVIBuffer_DCell* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _float3* pPoints);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void			Free() override;
};

END
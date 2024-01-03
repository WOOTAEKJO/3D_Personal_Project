#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CVIBuffer_Instancing abstract : public CVIBuffer
{
protected:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs);
	virtual	~CVIBuffer_Instancing() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual HRESULT	Bind_Buffer() override;
	virtual HRESULT	Render();

protected:
	ID3D11Buffer*	m_pInstanceBuffer = { nullptr };
	_uint			m_iInstanceStride = { 0 };
	_uint			m_iInstanceNum = { 0 };
	_uint			m_iIndexCountPerInstance = { 0 };

public:
	virtual	CComponent* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
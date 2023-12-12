#pragma once
#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CCell() = default;

public:
	HRESULT	Initialize(FLOAT3X3 pPoints);

#ifdef _DEBUG
	HRESULT	Render(class CShader* pShader, _float4x4 matView, _float4x4 matProj);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

private:
	class CVIBuffer_Cell*	m_pBufferCom = { nullptr };

#endif

private:
	_float3	m_pPoints[3];

public:
	static	CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,FLOAT3X3 pPoints );
	virtual	void	Free() override;
};

END
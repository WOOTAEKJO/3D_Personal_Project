#pragma once
#include "Bounding.h"

BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef	struct tagBounding_OBB_Desc : public BOUNDING_DESC
	{
		_float3	vExtents;
		_float3 vRotation;

	}BOUNDING_OBB_DESC;

private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CBounding_OBB() = default;

public:
	HRESULT	Initialize(BOUNDING_DESC* Bounding_Desc);
	virtual	HRESULT	Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;

private:
	BoundingOrientedBox*	m_pOriOBB = { nullptr };
	BoundingOrientedBox*	m_pOBB = { nullptr };

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc);
	virtual	void	Free() override;
};

END
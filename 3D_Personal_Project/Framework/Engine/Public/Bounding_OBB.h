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
	virtual	void	Update(_fmatrix	matWorld) override;
	virtual	HRESULT	Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
	virtual _bool	Collision(class CCollider* pTargetCollider, _float3* vCollisionDir,
		_float* fPushedDist, _float4* vColliderPos) override;

public:
	BoundingOrientedBox* Get_BoundingOBB() { return m_pOBB; }

private:
	BoundingOrientedBox*	m_pOriOBB = { nullptr };
	BoundingOrientedBox*	m_pOBB = { nullptr };

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc);
	virtual	void	Free() override;
};

END
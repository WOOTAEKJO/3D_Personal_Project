#pragma once
#include "Bounding.h"

BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef	struct tagBounding_AABB_Desc : public BOUNDING_DESC
	{
		_float3	vExtents;

	}BOUNDING_AABB_DESC;

private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CBounding_AABB() = default;

public:
	HRESULT	Initialize(BOUNDING_DESC* Bounding_Desc);
	virtual	void	Update(_fmatrix	matWorld) override;
	virtual _bool	Collision(class CCollider* pTargetCollider, _float3* vCollisionDir,
		_float* fPushedDist, _float4* vColliderPos) override;

#ifdef _DEBUG
	virtual	HRESULT	Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

public:
	BoundingBox*	Get_BoundingAABB() { return m_pAABB; }

private:
	BoundingBox*	m_pOriAABB = { nullptr };
	BoundingBox*	m_pAABB = { nullptr };

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc);
	virtual	void	Free() override;
};

END
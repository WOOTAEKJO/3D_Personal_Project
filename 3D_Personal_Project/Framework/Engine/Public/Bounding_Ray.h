#pragma once
#include "Bounding.h"

BEGIN(Engine)

class CBounding_Ray final : public CBounding
{
public:
	typedef	struct tagBounding_Ray_Desc : public BOUNDING_DESC
	{
		_float3 vDir;

	}BOUNDING_RAY_DESC;

private:
	CBounding_Ray(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CBounding_Ray() = default;

public:
	HRESULT	Initialize(BOUNDING_DESC* Bounding_Desc);
	virtual	void	Update(_fmatrix	matWorld) override;
	virtual	HRESULT	Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
	virtual _bool	Collision(class CCollider* pTargetCollider, _float3* vCollisionDir,
		_float* fPushedDist, _float4* vColliderPos) override;

public:
	BOUNDING_RAY_DESC		Get_BoundingRay() { return m_Ray; }

private:
	BOUNDING_RAY_DESC	m_Ray;
	BOUNDING_RAY_DESC	m_OriginRay;

public:
	static CBounding_Ray* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc);
	virtual	void	Free() override;
};

END
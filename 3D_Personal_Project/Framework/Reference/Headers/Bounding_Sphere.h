#pragma once
#include "Bounding.h"

BEGIN(Engine)

class CBounding_Sphere final : public CBounding
{
public:
	typedef	struct tagBounding_Sphere_Desc : public BOUNDING_DESC
	{
		_float fRadius;

	}BOUNDING_SPHERE_DESC;

private:
	CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CBounding_Sphere() = default;

public:
	HRESULT	Initialize(BOUNDING_DESC* Bounding_Desc);
	virtual	HRESULT	Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;

private:
	BoundingSphere*	m_pOriSphere = { nullptr };
	BoundingSphere*	m_pSphere = { nullptr };

public:
	static CBounding_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc);
	virtual	void	Free() override;
};

END
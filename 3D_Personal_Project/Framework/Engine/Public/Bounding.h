#pragma once
#include "Base.h"

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

	typedef struct tagBounding_Desc
	{
		TYPE eType = TYPE_END;
		_float3	vCenter;

	}BOUNDING_DESC;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CBounding() = default;

public:
	virtual	void	Update(_fmatrix	matWorld) = 0;
	_bool	Colision(TYPE eType ,class CCollider* pTargetCollider);

#ifdef _DEBUG
public:
	virtual	HRESULT	Render(PrimitiveBatch<VertexPositionColor>* pBatch) = 0;
#endif

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	virtual	void	Free() override;
};

END
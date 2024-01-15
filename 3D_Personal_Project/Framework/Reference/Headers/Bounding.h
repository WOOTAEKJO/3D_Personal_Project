#pragma once
#include "Base.h"

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

	typedef struct tagBounding_Desc
	{
		class CGameObject* pOnwer = nullptr;
		TYPE		eType = TYPE_END;
		_float3		vCenter;
		_bool		bUseCol = false;

	}BOUNDING_DESC;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CBounding() = default;

public:
	virtual	void	Update(_fmatrix	matWorld) = 0;
	virtual _bool	Collision(class CCollider* pTargetCollider, _float3* vCollisionDir,_float* fPushedDist) = 0;

#ifdef _DEBUG
public:
	virtual	HRESULT	Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) = 0;
#endif

public:
	TYPE	Get_ColisionType() { return m_eType; }
	_float4x4	Get_CollsionWorldMat() { return m_matWorld; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	TYPE					m_eType = { TYPE_END };

	_float4x4				m_matWorld;

public:
	virtual	void	Free() override;
};

END
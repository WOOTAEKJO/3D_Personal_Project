#pragma once
#include "Component.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg) override;

#ifdef _DEBUG

	HRESULT	Render();

#endif

private:
	CBounding::TYPE			m_eType = { CBounding::TYPE_END };
	CBounding*		m_pBounding = { nullptr };

#ifdef _DEBUG

private:
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*							m_pEffect = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };

#endif

public:
	static	CCollider* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
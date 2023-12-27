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

	void	Update(_fmatrix	matWorld);
#ifdef _DEBUG
	HRESULT	Render();
#endif

	_bool	Collision(class CCollider* pTargetCollider);

public:
	CBounding* Get_Bounding() { return m_pBounding; }
	_bool		Get_UseCol() { return m_bUseCol; }
	_bool		Get_Collision() { return m_bCollision; }
	_uint		Get_ColLayer_Type() { return m_eColLayer_Type; }
	_uint		Get_Collider_ID() { return m_iColID; }

	void		Set_ColLayer_Type(_uint iType) { m_eColLayer_Type = iType; }
	void		Set_UseCol(_bool bCheck) { m_bUseCol = bCheck; }
	void		Set_Collision(_bool bCheck) { m_bCollision = bCheck; }
public:
	void	OnCollisionEnter(CCollider* pOtherCollider);
	void	OnCollisionStay(CCollider* pOtherCollider);
	void	OnCollisionExit(CCollider* pOtherCollider);

private:
	CBounding*				m_pBounding = { nullptr };
	//CGameObject*				m_pOnwer = { nullptr };

#ifdef _DEBUG

private:
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*							m_pEffect = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };

#endif

private:
	_bool					m_bUseCol = { false };
	_bool					m_bCollision = { false };
	_uint					m_eColLayer_Type = { 0 };
	_uint					m_iColID = { 0 };

private:
	static _uint			m_iNextID;

public:
	static	CCollider* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
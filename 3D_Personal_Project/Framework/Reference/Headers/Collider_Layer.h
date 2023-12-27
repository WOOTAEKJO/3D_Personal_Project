#pragma once
#include "Base.h"

BEGIN(Engine)

class CCollider;

class CCollider_Layer final : public CBase
{
private:
	CCollider_Layer();
	virtual	~CCollider_Layer() = default;

public:
	HRESULT	Add_Collider(CCollider* pCollider);
	void	Delete_Collider();

public:
	list<CCollider*>& Get_ColliderList() { return m_listCollider; }

private:
	list<CCollider*>	m_listCollider;

public:
	static	CCollider_Layer* Create();
	virtual	void	Free() override;
};

END
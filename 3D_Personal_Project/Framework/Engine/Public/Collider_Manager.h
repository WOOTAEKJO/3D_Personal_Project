#pragma once
#include "Base.h"
#include "Collider.h"

union COLLIDER_ID
{
	struct
	{
		_uint iSour_ID;
		_uint iDest_ID;
	};
	_ulonglong ID;
};

BEGIN(Engine)

class CCollider_Layer;

class CCollider_Manager final : public CBase
{
private:
	CCollider_Manager();
	virtual	~CCollider_Manager() = default;

public:
	HRESULT	Initialize();
	void	Update();
	void	Delete_Collider();

public:
	HRESULT	Add_Collision(_uint iColLayer, CCollider* pCollider);
	HRESULT	Add_Pair_Collision(_uint iSourColLayer, _uint iDestColLayer);
	
private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	map<_uint, CCollider_Layer*>		m_mapColliderLayer;
	vector<pair<CCollider_Layer*, CCollider_Layer*>> m_vecActiveCollider;
	map<_ulonglong, _bool>				m_mapJudgeColl;

private:
	void				Check_Collision(CCollider* SourpCollider, CCollider* DestpCollider);
	CCollider_Layer* Find_Collider_Layer(_uint iColLayer);

public:
	static	CCollider_Manager* Create();
	virtual	void	Free() override;
};
	

END
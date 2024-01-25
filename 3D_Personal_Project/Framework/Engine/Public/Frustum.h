#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameInstance;

class CFrustum final :public CBase
{
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT	Initialize();
	void	Tick();

public:
	void	Transform_ToLocalSpace(_fmatrix matWorld);
	_bool	IsIn_World_FrustumPlanes(_fvector vPoint, _float fRadius);
	_bool	IsIn_LocalPlanes(_fvector vPoint, _float fRadius);

private:
	CGameInstance* m_pGameInstance = { nullptr };

private:
	_float3		m_vOriginalPoints[8];
	_float4		m_WorldPlanes[6];
	_float4		m_LocalPlanes[6];

private:
	void	Make_Planes(const _float3* pPoints, _Out_ _float4* pPlanes);

public:
	static CFrustum* Create();
	virtual void	Free();
};

END
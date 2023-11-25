#pragma once
#include "Base.h"

BEGIN(Engine)

class CMouseManager final : public	CBase
{
private:
	CMouseManager();
	virtual	~CMouseManager() = default;

public:
	HRESULT	Initialize();
	void	Update_Mouse(_float4x4 matView, _float4x4 matProj, HWND hWnd);
	_bool	Intersect(RAY tRay, _float3* pOut, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld);

private:
	RAY		m_pRay;

public:
	static	CMouseManager* Create();
	virtual	void	Free() override;
};

END
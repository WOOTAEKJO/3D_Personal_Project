#pragma once
#include "Base.h"

BEGIN(Engine)

class CMouse_Manager final : public	CBase
{
private:
	CMouse_Manager();
	virtual	~CMouse_Manager() = default;

public:
	HRESULT	Initialize();
	void	Update_Mouse(_float4x4 matView, _float4x4 matProj, HWND hWnd);
	_bool	Intersect(_float3* pOut, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld);

private:
	RAY		m_pRay;

public:
	static	CMouse_Manager* Create();
	virtual	void	Free() override;
};

END
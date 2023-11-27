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
	void	Update_Mouse(HWND hWnd);
	_bool	Intersect(_float3* pOut, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	RAY		m_pRay;

public:
	static	CMouse_Manager* Create();
	virtual	void	Free() override;
};

END
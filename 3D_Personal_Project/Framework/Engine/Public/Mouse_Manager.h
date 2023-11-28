#pragma once
#include "Base.h"

BEGIN(Engine)

class CMouse_Manager final : public	CBase
{
private:
	CMouse_Manager();
	virtual	~CMouse_Manager() = default;

public:
	HRESULT	Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	void	Update_Mouse();
	_bool	Intersect(_float3* pOut, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld);
	void	Mouse_Fix();

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	HWND	m_hWnd;
	_uint	m_iWinSize[2] = {};

private:
	RAY		m_pRay;

public:
	static	CMouse_Manager* Create(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	virtual	void	Free() override;
};

END
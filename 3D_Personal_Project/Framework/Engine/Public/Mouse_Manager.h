#pragma once
#include "Base.h"

BEGIN(Engine)

class CTransform;

class CMouse_Manager final : public	CBase
{
private:
	CMouse_Manager();
	virtual	~CMouse_Manager() = default;

public:
	HRESULT	Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	void	Update_Mouse();
	_bool	Intersect(_float3* pOut,_float* fDist, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld);
	_bool	Intersect_Sphere(BoundingSphere* pSphere,_float* fDist);
	
	void	Free_Mouse(_float fTimeDelta, _float fMouseSensitivity, CTransform* pTransCom);

	void	Mouse_Move(_float fTimeDelta, _float fMouseSensitivity, CTransform* pTransCom);

	_float4	Get_WorldMouse_Float4() {
		return m_vWorldMouse;
	}

	_vector	Get_WorldMouse_Vector() {
		return XMLoadFloat4(&m_vWorldMouse);
	}

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	HWND	m_hWnd;
	_uint	m_iWinSize[2] = {};

private:
	RAY		m_pRay;

private:
	_bool	m_bFix = { true };
	_bool	m_bCheck = { true };

private:
	_float4	m_vWorldMouse = {};

private:
	void	Mouse_Fix();
	void	Mouse_Key();

public:
	static	CMouse_Manager* Create(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	virtual	void	Free() override;
};

END
#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_float4 vEye, vAte;
		_float fFovy, fAspect, fNear, fFar;

	}CAMERADESC;
protected:
	CCamera(ID3D11Device* pDevice , ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual	~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

public:
	void	Camera_Sliding(_fvector vPosition,CNavigation* pNavigation,_float fTimeDelta);
	_vector	Camera_Spring(_fvector vTargetPos, _fvector vPos, _float fTimeDelta ,_float fRatio = 0.2f);

protected:
	_float m_fFovy = { 0.f };
	_float m_fAspect = { 0.f };
	_float m_fNear = { 0.f };
	_float m_fFar = { 0.f };

protected:
	_float			m_fSpringConstant = { 0.f };
	_float			m_fDampConstant = { 0.f };
	_float3			m_vVeclocity;

protected:
	_bool			m_bWallCheck = { false };

public:
	virtual	CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
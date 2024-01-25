#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CGameObject;
class CTransform;

END

BEGIN(Client)

class CTargetCamera final : public CCamera
{
public:
	typedef struct tagTargetCameraDesc : public CCamera::CAMERADESC
	{

	}TARGETCAMERADESC;
private:
	CTargetCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTargetCamera(const CTargetCamera& rhs);
	virtual	~CTargetCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

public:
	void	SetUp_Offset(_float3 vOffset) { m_vOffset = vOffset; }
	void	Reset_Offset() { m_vOffset = m_vOriginOffset; }

	void	SetUp_LookAt_Height(_float fHeight) { m_fLookAt_Height = fHeight; }
	void	Reset_LookAt_Height() { m_fLookAt_Height = m_fOriginLookAt_Height; }

private:
	CGameObject* m_pTarget = { nullptr };
	CTransform* m_pTargetTransform = { nullptr };
	CStateMachine* m_pTargetStateMachine = { nullptr };

private:
	CNavigation* m_pNavigationCom = { nullptr };

private:
	_float			m_fAngleAccY = { 0.f };
	_float			m_fAngleAccX = { 0.f };

	_float			m_fOriginLookAt_Height = { 1.3f };
	_float			m_fLookAt_Height;

	_float3			m_vOriginOffset = { 0.7f,-0.5f,0.7f };
	_float3			m_vOffset;
private:
	wstring			m_strCurrentNaviTag;

private:
	_float4			m_vPrevTargetPos;

private:
	_bool			m_bStateTrans = { false };
	_float			m_fTransAcc = { 0.f };

private:
	HRESULT	Ready_Component();
	void	StateTrans(_float fTimeDelta);

private:
	void	Mouse_Input(_float fTimeDelta);
	void	Mouse_Fix();

public:
	static	CTargetCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
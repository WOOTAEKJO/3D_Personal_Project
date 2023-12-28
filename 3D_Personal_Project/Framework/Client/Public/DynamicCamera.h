#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CDynamicCamera final : public CCamera
{
public:
	typedef struct tagDynamicCameraDesc : public CCamera::CAMERADESC
	{

		_float	fMouseSensitivity;

	}DYNAMICCAMERADESC;
private:
	CDynamicCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamicCamera(const CDynamicCamera& rhs);
	virtual	~CDynamicCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

private:
	_float m_fMouseSensitivity = { 0.f };

private:
	void	Key_Input(_float fTimeDelta);

public:
	static	CDynamicCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
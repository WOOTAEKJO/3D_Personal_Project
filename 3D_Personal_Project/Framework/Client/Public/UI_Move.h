#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Move final : public CUI
{
public:
	typedef struct tagUI_Move_Desc : public CUI::UI_DESC
	{
		_float fSpeed;
		_float3 vDir;
		_float fAngle;

	}UI_MOVE_DESC;

private:
	CUI_Move(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Move(const CUI_Move& rhs);
	virtual	~CUI_Move() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float m_fSpeed = { 0.f };
	_float m_fAngle = { 0.f };
	_float3 m_vDir;

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	void	Move(_float fTimeDelta);

public:
	static CUI_Move* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
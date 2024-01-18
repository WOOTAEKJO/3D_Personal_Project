#pragma once
#include "Bullet.h"

BEGIN(Client)

class CBomb final : public CBullet
{
private:
	CBomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBomb(const CBomb& rhs);
	virtual	~CBomb() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Start_Point_Toward(_float fTimeDelta);
	void	Set_TartgetPos(_float4 vTargetPos);
	void	Set_Start(_bool bCheck) { m_bStart = bCheck; }

private:
	CModel* m_pModelCom = { nullptr };

private:
	_float4		m_vTargetPos;
	_bool		m_bForce = {true};
	_bool		m_bArrive = { true };
	_bool		m_bStart = { false };
	_bool		m_bFall = {false};

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CBomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
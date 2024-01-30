#pragma once
#include "Bullet.h"

BEGIN(Client)

class CNormal_Bullet final : public CBullet
{
public:
	typedef struct tagNormalBullet_Desc : public CBullet::BULLET_DESC
	{
		_float4 vTrailColor;

	}BULLET_NORMAL_DESC;

private:
	CNormal_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormal_Bullet(const CNormal_Bullet& rhs);
	virtual	~CNormal_Bullet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CGameObject* m_pTrailEffect = { nullptr };

private:
	_bool	m_bFall = { false };

public:
	void	Toward(_float fTimeDelta);
	void	Dead_Judge();

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();
	virtual HRESULT	Init_Point_Light() override;

public:
	static CNormal_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
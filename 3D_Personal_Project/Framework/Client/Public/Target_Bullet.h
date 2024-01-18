#pragma once
#include "Bullet.h"

BEGIN(Client)

class CTarget_Bullet final : public CBullet
{
private:
	CTarget_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTarget_Bullet(const CTarget_Bullet& rhs);
	virtual	~CTarget_Bullet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void	Toward(_float fTimeDelta);

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CTarget_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
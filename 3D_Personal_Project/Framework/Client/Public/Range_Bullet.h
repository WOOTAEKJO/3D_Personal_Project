#pragma once
#include "Bullet.h"

BEGIN(Client)

class CRange_Bullet final : public CBullet
{
private:
	CRange_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRange_Bullet(const CRange_Bullet& rhs);
	virtual	~CRange_Bullet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:


private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CRange_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
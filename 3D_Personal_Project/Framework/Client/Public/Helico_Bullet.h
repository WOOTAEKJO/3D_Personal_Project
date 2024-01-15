#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CHelico_Bullet final : public CBullet
{
private:
	CHelico_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHelico_Bullet(const CHelico_Bullet& rhs);
	virtual	~CHelico_Bullet() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Toward(_float fTimeDelta);
	void	LookAt_Y();

private:
	CModel* m_pModelCom = { nullptr };

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CHelico_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
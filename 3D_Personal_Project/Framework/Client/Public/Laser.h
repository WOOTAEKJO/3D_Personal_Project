#pragma once
#include "Bullet.h"

BEGIN(Client)

class CLaser final : public CBullet
{
private:
	CLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLaser(const CLaser& rhs);
	virtual	~CLaser() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

private:
	void	Update_MatWorlds(_float fTimeDelta);
	void	Update_TartgetPos();

public:
	static CLaser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
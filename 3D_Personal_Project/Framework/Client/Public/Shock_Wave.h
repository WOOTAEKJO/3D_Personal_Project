#pragma once
#include "Bullet.h"

BEGIN(Client)

class CShock_Wave final : public CBullet
{
private:
	CShock_Wave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShock_Wave(const CShock_Wave& rhs);
	virtual	~CShock_Wave() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float4x4		m_matWorlds[20];

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

private:
	void	Update_MatWorlds(_float fTimeDelta);
	void	Create_MatWorlds();

public:
	static CShock_Wave* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
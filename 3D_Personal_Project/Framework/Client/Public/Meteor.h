#pragma once
#include "Bullet.h"

BEGIN(Client)

class CMeteor final : public CBullet
{
private:
	CMeteor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeteor(const CMeteor& rhs);
	virtual	~CMeteor() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Fall(_float fTimeDelta);

private:
	CModel* m_pModelCom = { nullptr };

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CMeteor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
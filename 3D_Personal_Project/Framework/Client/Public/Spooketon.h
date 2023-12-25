#pragma once
#include "Monster.h"

BEGIN(Client)

class CSpooketon final : public CMonster
{
private:
	CSpooketon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpooketon(const CSpooketon& rhs);
	virtual	~CSpooketon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;
	virtual HRESULT	Ready_State() override;

public:
	static	CSpooketon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
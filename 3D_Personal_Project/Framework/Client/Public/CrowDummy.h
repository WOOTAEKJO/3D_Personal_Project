#pragma once
#include "NPC.h"

BEGIN(Client)

class CCrowDummy final : public CNPC
{	
private:
	CCrowDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCrowDummy(const CCrowDummy& rhs);
	virtual	~CCrowDummy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

public:
	static	CCrowDummy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
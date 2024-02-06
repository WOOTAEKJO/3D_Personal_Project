#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_ChatBox final : public CUI
{
private:
	CUI_ChatBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_ChatBox(const CUI_ChatBox& rhs);
	virtual	~CUI_ChatBox() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

public:
	static CUI_ChatBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_HP final : public CUI
{
public:
	typedef struct tagUI_HP_Desc : public CUI::UI_DESC
	{
		CGameObject* pOwner = nullptr;
		wstring strMaskTexture;
		wstring strNoiseTexture;
		_float4 vSolidColor;
		_uint	iShaderPassIndx;

	}UI_HP_DESC;

private:
	CUI_HP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_HP(const CUI_HP& rhs);
	virtual	~CUI_HP() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*	m_pMaskTexture = { nullptr };
	CTexture* m_pNoiseTexture = { nullptr };

private:
	CGameObject* m_pOwner = {nullptr};

private:
	wstring		m_pMaskTag;
	wstring		m_strNoiseTexture;
	_float4		m_vSolidColor;
	_float4		m_vHPColor;
	_float		m_fTimeDelta = { 0.f };
	_uint		m_iShaderPassIndx = { 0 };

private:
	_float		m_fHP_Ratio = { 0 };

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	void	Compute_Ratio();

public:
	static CUI_HP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Dissolve final : public CUI
{
public:
	typedef struct tagUI_Dissolve_Desc : public CUI::UI_DESC
	{
		wstring strDissolveTexture;

		_float fDissolveAmount = { 0.f };
		_float fDissolveGradiationDistance = {0.f};
		_float3 vDissolveGradiationStartColor;
		_float3 vDissolveGradiationGoalColor;

	}UI_DISSOLVE_DESC;

private:
	CUI_Dissolve(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Dissolve(const CUI_Dissolve& rhs);
	virtual	~CUI_Dissolve() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool	Get_Ready_Ok() { return m_bReady; }

private:
	CTexture* m_pDissolveTexture = { nullptr };

private:
	UI_DISSOLVE_DESC m_Dissolve_Desc;
	_float		m_fTimeDelta = { 0.f };

private:
	_bool m_bTrueRender = { false };
	_bool m_bReady = { false };

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	void	Dissolve_Render(_float fTimeDelta);

public:
	static CUI_Dissolve* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END
#pragma once
#include "Effect.h"

BEGIN(Client)

class CEffect_Light final : public CEffect
{
public:
	typedef struct tagEffect_Light_Info : public CEffect::EFFECTINFO
	{
		_float4 vSolid_Color;
		wstring	strMaskTag;
		_float	fAlpha;

		CBone* pSocketBone = nullptr;

	}EFFECT_LIGHT_INFO;
private:
	CEffect_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Light(const CEffect_Light& rhs);
	virtual	~CEffect_Light() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

private:
	CTexture* m_pMaskTextureCom = { nullptr };
	
private:
	CTransform* m_pOwnerTransform = { nullptr };

private:
	CBone* m_pSocketBone = { nullptr };

	_float4x4	m_matWorldMat = {};

private:
	EFFECT_LIGHT_INFO m_Light_Info;

private:
	virtual HRESULT	Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;

private:
	void	Update_Pos();

	void	Look_Cam();

public:
	static CEffect_Light* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg);
	virtual	void	Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "AlphaObject.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CVIBuffer_Rect;
class CVIBuffer_DRect;

END

BEGIN(Client)

class CEffect abstract : public CAlphaObject
{
public:
	typedef struct tagEffectInfo : public CGameObject::GAMEOBJECT_DESC
	{
		wstring			strEffectTextureTag;
		CGameObject* pOwner = nullptr;
		_float			fLifeTime = 0.f;
		_float4			vPos = { 0.f,0.f,0.f,1.f };
		_float2			vSize = { 1.f,1.f };
	}EFFECTINFO;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual	~CEffect() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

protected:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pBufferCom = { nullptr };
	CVIBuffer_DRect* m_pDBufferCom = { nullptr };

protected:
	CGameObject* m_pOwner = { nullptr };

protected:
	_bool		m_bSprite = { false };
	_float		m_fFrame = { 0.f };
	_float		m_fMaxFrame = { 0.f };

protected:
	wstring			m_strTextureTag;

protected:
	_float		m_fTimeAcc = { 0.f };
	_float		m_fLifeTime = { 0.f };

protected:
	_float4		m_vSolid_Color = { 1.f,1.f,1.f,1.f };
	_float		m_fAlpha = { 0.f };
	_float2		m_vSize = { 1.f,1.f };
	_float4		m_vPos = { 0.f,0.f,0.f,1.f };

protected:
	virtual HRESULT	Bind_ShaderResources();
	virtual HRESULT	Ready_Component();

	void	Judge_Dead(_float fTimeDelta);

	void	Size_Up(_float fTimeDelta);

	void	Invisibility(_float fTimeDelta);

public:
	virtual	CGameObject*	Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
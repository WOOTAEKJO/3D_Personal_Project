#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

END

BEGIN(Client)

class CUI abstract : public CGameObject
{
public:
	typedef struct tagUI_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_float2 vScale;
		_float2 vCenterPos;
		wstring strTextureTag;
		_bool	bRender = false;

	}UI_DESC;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual	~CUI() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_Render(_bool bCheck) { m_bRender = bCheck; }

protected:
	CTexture*			m_pTextureCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CVIBuffer_DRect*	m_pBufferCom = { nullptr };

protected:
	UI_DESC		m_tUI_Desc;

	_float4x4			m_ViewMatrix, m_ProjMatrix;

protected:
	_bool				m_bRender = {false};

protected:
	virtual HRESULT	Bind_ShaderResources();
	virtual HRESULT	Ready_Component();

public:
	virtual	CGameObject*	Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END
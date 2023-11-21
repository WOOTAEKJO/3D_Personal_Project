#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CShader;
class CTexture;

END

BEGIN(Client)

class CBackGround final : public CGameObject
{
public:
	typedef	struct tagBackGroundDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_float fX = 0.f;
		_float fY = 0.f;
		_float fSizeX = 0.f;
		_float fSizeY = 0.f;

	}BACKGROUND_DESC;

private:
	CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };

private:
	_float	m_fX = { 0.f };
	_float	m_fY = { 0.f };
	_float	m_fSizeX = { 0.f };
	_float	m_fSizeY = { 0.f };

	_float4x4			m_ViewMatrix, m_ProjMatrix;

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	/* 원형객체를 생성한다. */
	static CBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/* 사본객체를 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END
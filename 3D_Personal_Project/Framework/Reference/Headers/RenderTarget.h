#pragma once
#include "Base.h"

BEGIN(Engine)

class CShader;
class CVIBuffer_Rect;

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	HRESULT Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT Pixel_Format,_float4 vColor);
	HRESULT	Bind_ShaderResource(CShader* pShader, const _char* pConstantName);
	HRESULT	Clear();

#ifdef _DEBUG

	HRESULT	Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT	Render_Debug(CShader* pShader, CVIBuffer_Rect* pBuffer);

#endif

public:
	ID3D11RenderTargetView* Get_RTV() { return m_pRTV; }

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

private:
	ID3D11Texture2D*			m_pTexture2D = { nullptr };
	ID3D11RenderTargetView*		m_pRTV = { nullptr };
	ID3D11ShaderResourceView*	m_pSRV = { nullptr };

private:
	_float4x4					m_matWorld;

	_float4						m_vColor;

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		_uint iSizeX, _uint iSizeY, DXGI_FORMAT Pixel_Format, _float4 vColor);
	virtual	 void	Free() override;
};

END
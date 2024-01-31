#pragma once
#include "Base.h"

BEGIN(Engine)

class CRenderTarget;
class CGameInstance;

class CRenderTarget_Manager final : public CBase
{
private:
	CRenderTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget_Manager() = default;


public:
	HRESULT Initialize();
	HRESULT	Add_RenderTarget(RTV_TYPE eType, _uint iSizeX, _uint iSizeY, DXGI_FORMAT Pixel_Format, const _float4& vColor);
	HRESULT	Add_MRT(const wstring& strMRTTag, RTV_TYPE eType);
	HRESULT	Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSV);
	HRESULT	End_MRT();
	HRESULT Bind_ShaderResource(RTV_TYPE eType, CShader* pShader, const _char* pConstantName);

#ifdef _DEBUG
	HRESULT	Ready_Debug(RTV_TYPE eType,_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT	Render_Debug(const wstring& strMRTTag,CShader* pShader, CVIBuffer_Rect* pBuffer);

#endif

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };

private:
	map<RTV_TYPE, CRenderTarget*> m_mapRenderTarget;
	map<const wstring, list<CRenderTarget*>> m_mapMRT;

private:
	CRenderTarget* Find_RenderTarget(RTV_TYPE eType);
	list<CRenderTarget*>* Find_MRT(const wstring& strMRTTag);

public:
	static CRenderTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	void	Free() override;
};

END
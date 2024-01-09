#pragma once
#include "Base.h"

BEGIN(Engine)

class CRenderTarget;

class CRenderTarget_Manager final : public CBase
{
private:
	CRenderTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget_Manager() = default;


public:
	HRESULT Initialize();
	HRESULT	Add_RenderTarget(RENDERTARGET_TYPE eType, _uint iSizeX, _uint iSizeY, DXGI_FORMAT Pixel_Format, const _float4& vColor);
	HRESULT	Add_MRT(const wstring& strMRTTag, RENDERTARGET_TYPE eType);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	map<RENDERTARGET_TYPE, CRenderTarget*> m_mapRenderTarget;
	map<const wstring, list<CRenderTarget*>> m_mapMRT;

private:
	CRenderTarget* Find_RenderTarget(RENDERTARGET_TYPE eType);
	list<CRenderTarget*>* Find_MRT(const wstring& strMRTTag);

public:
	static CRenderTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	void	Free() override;
};

END
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(const wstring& strTextureAddress, const _uint& iTextureNum);
	virtual	HRESULT	Initialize(void* pArg) override;
	
public:
	HRESULT	Bind_ShaderResource(class CShader* pShader, const _char* pTextureName, const _uint& iSRVIndex);
	HRESULT	Bind_ShaderResources(class CShader* pShader, const _char* pTextureName);

private:
	vector<ID3D11ShaderResourceView*> m_vecSRV;
	_uint							  m_iTextureNum = { 0 };

public:
	static CTexture*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureAddress, const _uint& iTextureNum = 1 );
	virtual	CComponent* Clone(void* pArg) override;
	virtual	 void		Free() override;
};

END
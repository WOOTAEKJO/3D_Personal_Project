#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	:CComponent(rhs),
	m_vecSRV(rhs.m_vecSRV),
	m_iTextureNum(rhs.m_iTextureNum)
{
	for (auto& iter : m_vecSRV)
		Safe_AddRef(iter);
}

HRESULT CTexture::Initialize_ProtoType(const wstring& strTextureAddress, const _uint& iTextureNum)
{
	m_iTextureNum = iTextureNum;

	m_vecSRV.reserve(m_iTextureNum);

	for (_uint i = 0; i < m_iTextureNum; i++)
	{
		_tchar strPath[MAX_PATH] = TEXT("");

		wsprintf(strPath, strTextureAddress.c_str(), i);

		_tchar	strEXT[MAX_PATH] = TEXT("");

		_wsplitpath_s(strPath, nullptr, 0, nullptr, 0, nullptr, 0, strEXT, MAX_PATH);

		ID3D11ShaderResourceView* pSRV = { nullptr };

		HRESULT	hr = 0;

		if (!lstrcmp(strEXT, TEXT(".dds")))
		{
			hr=CreateDDSTextureFromFile(m_pDevice, strPath, nullptr, &pSRV);
		}
		else if (!lstrcmp(strEXT, TEXT(".tga")))
		{
			MSG_BOX("tga extension cannot be used");
			hr = -1;
		}
		else 
		{
			hr= CreateWICTextureFromFile(m_pDevice, strPath, nullptr, &pSRV);
		}


		if (FAILED(hr))
			return E_FAIL;

		m_vecSRV.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pTextureName, const _uint& iSRVIndex)
{
	return pShader->Bind_SRV(pTextureName,m_vecSRV[iSRVIndex]);
}

HRESULT CTexture::Bind_ShaderResources(CShader* pShader, const _char* pTextureName)
{
	return pShader->Bind_SRVS(pTextureName,&m_vecSRV.front(),m_iTextureNum);
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureAddress, const _uint& iTextureNum)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);



	if (FAILED(pInstance->Initialize_ProtoType(strTextureAddress, iTextureNum))) {
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& iter : m_vecSRV)
		Safe_Release(iter);
	m_vecSRV.clear();
}

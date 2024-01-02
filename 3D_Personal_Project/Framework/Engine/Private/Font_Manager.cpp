#include "..\Public\Font_Manager.h"
#include "CustomFont.h"

CFont_Manager::CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice), m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CFont_Manager::Initialize()
{
	return S_OK;
}

HRESULT CFont_Manager::Add_Font(_uint iFontTag, const wstring& strFontFilePath)
{
	if (Find_Font(iFontTag) != nullptr)
		return E_FAIL;

	m_mapFont.emplace(iFontTag, CCustomFont::Create(m_pDevice, m_pContext, strFontFilePath));

	return S_OK;
}

HRESULT CFont_Manager::Render(_uint iFontTag, const wstring& strText, _float2 vPosition, _fvector vColor, _float fScale, _float2 vOrigin, _float fRotation)
{
	CCustomFont* pFont = Find_Font(iFontTag);
	if (pFont == nullptr)
		return E_FAIL;

	return pFont->Render(strText, vPosition, vColor, fScale, vOrigin, fRotation);
}

CCustomFont* CFont_Manager::Find_Font(_uint iFontTag)
{
	auto& iter = m_mapFont.find(iFontTag);
	if(iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

CFont_Manager* CFont_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFont_Manager* pInstance = new CFont_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CFont_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont_Manager::Free()
{
	__super::Free();

	for (auto& iter : m_mapFont)
		Safe_Release(iter.second);
	m_mapFont.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

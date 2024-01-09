#include "..\Public\RenderTarget_Manager.h"
#include "RenderTarget.h"

CRenderTarget_Manager::CRenderTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget_Manager::Initialize()
{
	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_RenderTarget(RENDERTARGET_TYPE eType, _uint iSizeX, _uint iSizeY, DXGI_FORMAT Pixel_Format, const _float4& vColor)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(eType);
	if (pRenderTarget != nullptr)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(m_pDevice,m_pContext,iSizeX, iSizeY, Pixel_Format);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	m_mapRenderTarget.emplace(eType, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_MRT(const wstring& strMRTTag, RENDERTARGET_TYPE eType)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(eType);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	list<CRenderTarget*>* pMrt = Find_MRT(strMRTTag);
	if (pMrt == nullptr)
	{
		list<CRenderTarget*> ListMrt;

		ListMrt.push_back(pRenderTarget);

		m_mapMRT.emplace(strMRTTag, ListMrt);
	}
	else {
		pMrt->push_back(pRenderTarget);
	}

	return S_OK;
}

CRenderTarget* CRenderTarget_Manager::Find_RenderTarget(RENDERTARGET_TYPE eType)
{
	auto& iter = m_mapRenderTarget.find(eType);
	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CRenderTarget_Manager::Find_MRT(const wstring& strMRTTag)
{
	auto iter = m_mapMRT.find(strMRTTag);
	if (iter == m_mapMRT.end())
		return nullptr;

	return &(iter->second);
}

CRenderTarget_Manager* CRenderTarget_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderTarget_Manager* pInstance = new CRenderTarget_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CRenderTarget_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

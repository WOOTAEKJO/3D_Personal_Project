#include "..\Public\RenderTarget_Manager.h"
#include "RenderTarget.h"
#include "GameInstance.h"

CRenderTarget_Manager::CRenderTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),m_pContext(pContext), m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderTarget_Manager::Initialize()
{
	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_RenderTarget(RTV_TYPE eType, _uint iSizeX, _uint iSizeY, DXGI_FORMAT Pixel_Format, const _float4& vColor)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(eType);
	if (pRenderTarget != nullptr)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(m_pDevice,m_pContext,iSizeX, iSizeY, Pixel_Format, vColor);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	m_mapRenderTarget.emplace(eType, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_MRT(const wstring& strMRTTag, RTV_TYPE eType)
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

HRESULT CRenderTarget_Manager::Begin_MRT(const wstring& strMRTTag)
{
	list<CRenderTarget*>* pList =Find_MRT(strMRTTag);
	if (pList == nullptr)
		return E_FAIL;

	_uint iNum = 0;

	ID3D11RenderTargetView* pRenderTargets[8];

	for (auto& iter : *pList)
	{
		iter->Clear();
		pRenderTargets[iNum++] = iter->Get_RTV();
	}
	
	m_pContext->OMSetRenderTargets(iNum, pRenderTargets, m_pGameInstance->Get_DSV());

	return S_OK;
}

HRESULT CRenderTarget_Manager::End_MRT()
{
	ID3D11RenderTargetView* pRenderTargets[8];

	pRenderTargets[0] = m_pGameInstance->Get_BackBuffer();

	m_pContext->OMSetRenderTargets(1, pRenderTargets, m_pGameInstance->Get_DSV());

	return S_OK;
}

HRESULT CRenderTarget_Manager::Bind_ShaderResource(RTV_TYPE eType, CShader* pShader, const _char* pConstantName)
{
	CRenderTarget* pRTV = Find_RenderTarget(eType);
	if (pRTV == nullptr)
		return E_FAIL;

	return pRTV->Bind_ShaderResource(pShader, pConstantName);
}

#ifdef _DEBUG
HRESULT CRenderTarget_Manager::Ready_Debug(RTV_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRTV = Find_RenderTarget(eType);
	if (pRTV == nullptr)
		return E_FAIL;

	return pRTV->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CRenderTarget_Manager::Render_Debug(const wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pBuffer)
{
	list<CRenderTarget*>* pList = Find_MRT(strMRTTag);
	if (pList == nullptr)
		return E_FAIL;

	for (auto& iter : *pList)
	{
		if (FAILED(iter->Render_Debug(pShader, pBuffer)))
			return E_FAIL;
	}

	return S_OK;
}
#endif

CRenderTarget* CRenderTarget_Manager::Find_RenderTarget(RTV_TYPE eType)
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

	for (auto& List : m_mapMRT)
	{
		for (auto& RT : List.second)
			Safe_Release(RT);
		List.second.clear();
	}
	m_mapMRT.clear();

	for (auto& iter : m_mapRenderTarget)
		Safe_Release(iter.second);
	m_mapRenderTarget.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}

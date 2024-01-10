#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"

#include "VIBuffer_Rect.h"
#include "Shader.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),m_pContext(pContext), m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize()
{
	D3D11_VIEWPORT		Viewport;
	_uint				iNumViewport = { 1 };

	m_pContext->RSGetViewports(&iNumViewport, &Viewport);

	//Diffuse
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_DIFFUSE, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	//Normal
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_NORMAL, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	//Shader
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_SHADER, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObject"), RTV_TYPE::RT_DIFFUSE)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObject"), RTV_TYPE::RT_NORMAL)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), RTV_TYPE::RT_SHADER)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f));

#ifdef _DEBUG
	if (FAILED(Ready_RTV_Debug()))
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderID, CGameObject* pGameObject)
{
	if (pGameObject == nullptr ||
		eRenderID >= RENDERGROUP::RENDER_END)
		return E_FAIL;
		
	m_listRenderObject[eRenderID].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG

	/*if (FAILED(Render_Debug()))
		return E_FAIL;*/

#endif

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_PRIORITY]) {

		if(pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_NONLIGHT]) {

		if (pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	/*if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObject"))))
		return E_FAIL;*/

	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_NONBLEND]) {

		if (pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_NONBLEND].clear();

	/*if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_BLEND]) {

		if (pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_UI]) {

		if (pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_UI].clear();

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	if (FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_MRT_Debug(TEXT("MRT_GameObject"), m_pShader, m_pBufferCom)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_MRT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Ready_RTV_Debug()
{
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_DIFFUSE, 100.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_NORMAL, 100.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_SHADER, 300.f, 100.f, 200.f, 200.f)))
		return E_FAIL;

	return S_OK;
}

#endif

HRESULT CRenderer::Ready_Component()
{
	m_pBufferCom = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (m_pBufferCom == nullptr)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext,
		TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iElementsNum);
	if (m_pShader == nullptr)
		return E_FAIL;

	return S_OK;
}

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pBufferCom);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}

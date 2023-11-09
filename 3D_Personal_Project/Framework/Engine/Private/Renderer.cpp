#include "..\Public\Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
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
	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_NONBLEND]) {

		if (pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_NONBLEND].clear();

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
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

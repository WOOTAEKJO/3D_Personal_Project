#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"

#include "VIBuffer_Rect.h"
#include "Shader.h"

#include "AlphaObject.h"

_uint		g_iSizeX = 8192;
_uint		g_iSizeY = 4608;

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

#pragma region MRT_GAMEOBJECTS

	//Diffuse
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_DIFFUSE, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
	//Normal
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_NORMAL, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	//Depth
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_DEPTH, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObject"), RTV_TYPE::RT_DIFFUSE)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObject"), RTV_TYPE::RT_NORMAL)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObject"), RTV_TYPE::RT_DEPTH)))
		return E_FAIL;

#pragma endregion

#pragma region MRT_LightAcc
	//Shader
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_SHADE, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	//Specular
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_SPECULAR, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), RTV_TYPE::RT_SHADE)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), RTV_TYPE::RT_SPECULAR)))
		return E_FAIL;

#pragma endregion

#pragma region MRT_Shadow
	//LightDepth
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_LIGHTDEPTH, g_iSizeX, g_iSizeY,
		DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL; // 쉐도우 렌더 타겟 사이즈 최대로

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Shadow"), RTV_TYPE::RT_LIGHTDEPTH)))
		return E_FAIL; // 쉐도우 렌더 타겟 그룹 추가

	if (FAILED(Create_DepthStencil()))
		return E_FAIL; // 쉐도우 렌더 타겟 깊이 버퍼


#pragma endregion

#pragma region MRT_Bloom

	//Target_Effect
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_EFFECT, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)))) return E_FAIL;
	//Target_Blur_X
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_BLUR_X, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)))) return E_FAIL;
	//Target_Blur_Y
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_BLUR_Y, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Effect"), RTV_TYPE::RT_EFFECT)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_X"), RTV_TYPE::RT_BLUR_X)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_Y"), RTV_TYPE::RT_BLUR_Y)))
		return E_FAIL;

#pragma endregion

#pragma region MRT_Bloom
	//Target_Result
	if (FAILED(m_pGameInstance->Add_RenderTarget(RTV_TYPE::RT_RESULT, Viewport.Width, Viewport.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f)))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Result"), RTV_TYPE::RT_RESULT)))
		return E_FAIL;

#pragma endregion
	

	if (FAILED(Ready_Component()))
		return E_FAIL;

	XMStoreFloat4x4(&m_matWorld, XMMatrixScaling(Viewport.Width, Viewport.Height, 1.f));
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
	if (!pGameObject->Get_In_WorldPlanes())
		return S_OK;

	if (pGameObject == nullptr || pGameObject->Get_Dead() ||
		eRenderID >= RENDERGROUP::RENDER_END)
		return E_FAIL;
		
	m_listRenderObject[eRenderID].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_DebugRender(CComponent* pComponent)
{

#ifdef _DEBUG
	if (pComponent == nullptr)
		return E_FAIL;

	m_listComponent.push_back(pComponent);

	Safe_AddRef(pComponent);

#endif

	return S_OK;

}



HRESULT CRenderer::Draw_RenderGroup()
{

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_Shadow()))
		return E_FAIL;

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_LightAcc()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;

	if (FAILED(Render_Deferred()))
		return E_FAIL;

	if (FAILED(Render_Bloom()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG

	if (FAILED(Render_Debug()))
		return E_FAIL;

#endif

	return S_OK;
}

void CRenderer::Fog_SetUp(_float2 vForStart_End, _float4 vFogColor)
{
	m_vFogPosition = vForStart_End;
	m_vFogColor = vFogColor;
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

HRESULT CRenderer::Render_Shadow()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Shadow"), m_pLightDepthDSV)))
		return E_FAIL; // 쉐도우 렌더 타겟 그룹 바인딩

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = g_iSizeX;
	ViewPortDesc.Height = g_iSizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);
	// 쉐도우 렌더 타겟은 사이즈가 다르기 때문에 뷰포트를 사이즈에 맞게 리셋 시켜야 한다.

	for (auto& pGameObject : m_listRenderObject[RENDER_SHADOW])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Shadow();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDER_SHADOW].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL; // 쉐도우 렌더 타겟 바인딩 해제

	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = 1280;
	ViewPortDesc.Height = 720;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);
	// 쉐도우 렌더 타겟은 기록이 끝나면 원래 사이즈로 리셋 시킨다.

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Effect"))))
		return E_FAIL;

	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_NONLIGHT]) {

		if (pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_NONLIGHT].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObject"))))
		return E_FAIL;

	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_NONBLEND]) {

		if (pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_NONBLEND].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	/*
		- 백버퍼를 장치에 빼주고 Diffuse, Normal, Depth를 장치에 바인딩
		- 객체 렌더 후 다시 백버퍼를 장치에 바인딩
	*/

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_matWorld", &m_matWorld))) return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView))) return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj))) return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_matViewInv",
		&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::TRANSFORMSTATE::VIEW)))) return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matProjInv",
		&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::TRANSFORMSTATE::PROJ)))) return E_FAIL;

	_float fFar = 1000.f;
	if (FAILED(m_pShader->Bind_RawValue("g_fFar", &fFar, sizeof(_float)))) return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_vCameraPos",
		&m_pGameInstance->Get_CameraState(CPipeLine::CAM_POS), sizeof(_float4)))) return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_NORMAL, m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_DEPTH, m_pShader, "g_DepthTexture")))
		return E_FAIL;

	if(FAILED(m_pGameInstance->Render_Light(m_pShader, m_pBufferCom))) return E_FAIL;

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
	if (FAILED(m_pShader->Bind_Matrix("g_matWorld", &m_matWorld))) return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView))) return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj))) return E_FAIL;

	m_pGameInstance->Get_ShadowLight()->SetUp_Matrix();

	if (FAILED(m_pShader->Bind_Matrix("g_matLightView", &m_pGameInstance->Get_ShadowLight()
	->Get_Matrix(CShadowLight::STATE::VIEW)))) return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matLightProj", &m_pGameInstance->Get_ShadowLight()
		->Get_Matrix(CShadowLight::STATE::PROJ)))) return E_FAIL;

	_float fFar = m_pGameInstance->Get_ShadowLight()->Open_Light_Desc()->fFar;

	if (FAILED(m_pShader->Bind_RawValue("g_fLightFar", &fFar,
		sizeof(_float)))) return E_FAIL;
	//// 쉐도우 렌더 타겟 far 값 바인딩

	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_SHADE, m_pShader, "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_DIFFUSE, m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_SPECULAR, m_pShader, "g_SpecularTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_DEPTH, m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_LIGHTDEPTH, m_pShader, "g_LightDepthTexture")))
		return E_FAIL; // 쉐도우 렌더 타겟 바인딩

	if (FAILED(m_pShader->Bind_RawValue("g_fFogStart", &m_vFogPosition.x, sizeof(_float)))) return E_FAIL;
	if (FAILED(m_pShader->Bind_RawValue("g_fFogEnd", &m_vFogPosition.y, sizeof(_float)))) return E_FAIL;
	if (FAILED(m_pShader->Bind_RawValue("g_vFogColor", &m_vFogColor, sizeof(_float4)))) return E_FAIL;

	_float fCamFar = 1000.f;
	if (FAILED(m_pShader->Bind_RawValue("g_fFar", &fCamFar, sizeof(_float)))) return E_FAIL;

	m_pShader->Begin(3);

	m_pBufferCom->Bind_Buffer();

	return m_pBufferCom->Render();
}

HRESULT CRenderer::Render_Bloom()
{
	if (FAILED(m_pShader->Bind_Matrix("g_matWorld", &m_matWorld))) return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matView", &m_matView))) return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_matProj", &m_matProj))) return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_X"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_EFFECT, m_pShader, "g_EffectTexture")))
		return E_FAIL;

	m_pShader->Begin(6);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_Y"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_BLUR_X, m_pShader, "g_EffectTexture")))
		return E_FAIL;

	m_pShader->Begin(6);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	/*if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Effect"))))
		return E_FAIL;*/

	m_listRenderObject[RENDER_BLEND].sort([](CGameObject* pSour, CGameObject* pDest) {

		return (dynamic_cast<CAlphaObject*>(pSour)->Get_CamDinstance() >
			dynamic_cast<CAlphaObject*>(pDest)->Get_CamDinstance());

		});

	for (auto& pGameObject : m_listRenderObject[RENDERGROUP::RENDER_BLEND]) {

		if (pGameObject != nullptr)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_listRenderObject[RENDERGROUP::RENDER_BLEND].clear();

	/*if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;*/

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


	for (auto& iter : m_listComponent)
	{
		iter->Render();
		Safe_Release(iter);
	}
	m_listComponent.clear();

	/*if (FAILED(m_pGameInstance->Render_MRT_Debug(TEXT("MRT_GameObject"), m_pShader, m_pBufferCom)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_MRT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pBufferCom)))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Shadow"), m_pShader, m_pBufferCom)))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Effect"), m_pShader, m_pBufferCom)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Blur_X"), m_pShader, m_pBufferCom)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Blur_Y"), m_pShader, m_pBufferCom)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CRenderer::Ready_RTV_Debug()
{
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_DIFFUSE, 100.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_NORMAL, 100.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_DEPTH, 100.f, 500.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_SHADE, 300.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_SPECULAR, 300.f, 300.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_LIGHTDEPTH, 1130.f, 150.f, 300.f, 300.f)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_EFFECT, 300.f, 500.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_BLUR_X, 500.f, 500.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RTV_Debug(RTV_TYPE::RT_BLUR_Y, 700.f, 500.f, 200.f, 200.f)))
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

HRESULT CRenderer::Create_DepthStencil()
{
	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = g_iSizeX;
	TextureDesc.Height = g_iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr,
		&pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr,
		&m_pLightDepthDSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

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
#ifdef _DEBUG
	for (auto& iter : m_listComponent)
		Safe_Release(iter);
	m_listComponent.clear();
#endif 

	Safe_Release(m_pShader);
	Safe_Release(m_pBufferCom);

	Safe_Release(m_pLightDepthDSV);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

}

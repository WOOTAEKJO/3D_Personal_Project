#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "..\Public\MainApp.h"

#include "ImGuiMgr.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CMainApp::CMainApp()	
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);

}

HRESULT CMainApp::Initialize()
{	
	GRAPHIC_DESC		GraphicDesc = {};

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.iBackBufferSizeX = g_iWinSizeX;
	GraphicDesc.iBackBufferSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END,GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_ProtoType_Component_ForStaticLevel()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	io->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io->Fonts->GetGlyphRangesKorean());

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext); //  레퍼런스 증가 해줘야 하나??

	if (FAILED(CImGuiMgr::GetInstance()->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	
	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	/*bool bDemo = true;
	ImGui::ShowDemoWindow(&bDemo);*/
	//CImGuiMgr::GetInstance()->Tool();

	ImGui::Render();

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	/* 그려야할 모델들을 그리낟.*/	
	m_pGameInstance->Render_Engine();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	// Update and Render additional Platform Windows
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eStartLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* 무조건 로딩레벨부터 시작ㅇ르 할꺼야 .*/
	CLevel*		pLevel = CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID);
	if (nullptr == pLevel)
		return E_FAIL;

	m_pGameInstance->Open_Level(LEVEL_LOADING,pLevel);

	return S_OK;
}

HRESULT CMainApp::Ready_ProtoType_Component_ForStaticLevel()
{
	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VTXPOSTEX*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXPOSTEX"),
		CShader::Create(m_pDevice,m_pContext,TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxPosTex.hlsl"),VTXPOSTEX::Elements,VTXPOSTEX::iElementsNum))))
		return E_FAIL;

	/* For.Prototype_Component_StateMachine*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CImGuiMgr::GetInstance()->DestroyInstance();

	/*  내 멤버를 정리하면. */
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();

}


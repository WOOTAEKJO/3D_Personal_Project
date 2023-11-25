#include "stdafx.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "..\Public\ImGuiMgr.h"

#include "commdlg.h"
#include "shlwapi.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CImGuiMgr)

CImGuiMgr::CImGuiMgr() 
{
}

HRESULT CImGuiMgr::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;
    m_pGameInstance = CGameInstance::GetInstance();

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

#pragma region 도킹
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    ////io.ConfigViewportsNoAutoMerge = true;
    ////io.ConfigViewportsNoTaskBarIcon = true;
    //io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
#pragma endregion

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	return S_OK;
}

HRESULT CImGuiMgr::Render()
{
#pragma region 도킹
    //ImGui_ImplDX11_NewFrame();
    //ImGui_ImplWin32_NewFrame();
    //ImGui::NewFrame();

    ///*bool bDemo = true;
    //ImGui::ShowDemoWindow(&bDemo);*/
    ////CImGuiMgr::GetInstance()->Tool();

    //ImGui::Render();

    //ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    //// Update and Render additional Platform Windows
    //if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //{
    //	ImGui::UpdatePlatformWindows();
    //	ImGui::RenderPlatformWindowsDefault();
    //}
#pragma endregion
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::SetNextWindowSize(ImVec2(g_iWinSizeX, g_iWinSizeY), ImGuiCond_Always);
    ImGui::Begin("Dock", nullptr, window_flags);
    ImGui::PopStyleVar(2);
    ImGuiID dockspaceID = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspaceID, ImVec2(0, 0), dockspaceFlags);
    ImGui::End();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{


			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("PhysX_Collider", "PgUp"))
			{
				
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Level"))
		{
			if (ImGui::MenuItem("-1. Loading"))
			{
				
			}

			if (ImGui::MenuItem("00. Logo"))
			{
				
			}

			if (ImGui::MenuItem("01. GamePlay"))
			{
				
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editer"))
		{
			if (ImGui::MenuItem("Scene Editer"))
			{
				
			}

			if (ImGui::MenuItem("Effect Editer"))
			{
				
			}

			if (ImGui::MenuItem("Model Editer"))
			{
				
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

    Tool();

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

void CImGuiMgr::Tool()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
    ImVec4 vBackGroundColor = ImVec4(1.f,1.f,1.f,1.f);
    ImVec2 vWinSize = {300.f,500.f};
    string strName = "TEST";
    
    ImGui::PushStyleColor(ImGuiCol_PopupBg, vBackGroundColor);

    ImGui::SetNextWindowSize(vWinSize, 0);

    ImGui::Begin(strName.c_str(), 0, window_flags);

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("TEST", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Test1"))
        {
            

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Test2"))
        {
           

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }


    ImGui::PopStyleColor();

    ImGui::End();
}

void CImGuiMgr::Free()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	__super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}

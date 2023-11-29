#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"

#include "../Public/ImGuiMgr.h"

#include "commdlg.h"
#include "shlwapi.h"

#include "GameInstance.h"

#include "../Public/Terrain_Window.h"
#include "../Public/ImGui_Window.h"

#include "Terrain_Demo.h"

IMPLEMENT_SINGLETON(CImGuiMgr)

typedef struct tagImGuiMGR_Window_Desc
{
    string	strName;	// 창 이름
    ImGuiWindowFlags window_flags;	// 창 옵션
    ImVec2	vWinSize;	// 창 사이즈
    ImVec4 vBackGroundColor = ImVec4(1.f, 1.f, 1.f, 1.f);  // 백 창 색

}IMGUIMGRWINDESC;

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

    if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL,TEXT("BackGround"),
        TEXT("Prototype_GameObject_Terrain_Demo"), nullptr,reinterpret_cast<CGameObject**>(&m_pTerrain))))
        return E_FAIL;
    Safe_AddRef(m_pTerrain);

	return S_OK;
}

void CImGuiMgr::Tick()
{
    for (auto& iter : m_vecWindow) {
        if (iter != nullptr) {
            iter->Tick();
        }
    }
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
		if (ImGui::BeginMenu("Terrain"))
		{
			/*if (ImGui::MenuItem("Save"))
			{
			}*/

            Set_Terrain_Edit();

            ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Object"))
		{
			
            ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Camera"))
		{
			
            ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("UI"))
		{
			
            ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("Effect"))
        {

            ImGui::EndMenu();
        }

		ImGui::EndMainMenuBar();
	}

    for (size_t i = 0; i < m_vecWindow.size(); i++)
    {
        m_vecWindow[i]->Render();
    }

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

HRESULT CImGuiMgr::Create_HeightMap(_uint iX, _uint iZ)
{
    if (m_pTerrain == nullptr)
        return E_FAIL;

    if (FAILED(m_pTerrain->Create_DynamicBuffer(iX, iZ)))
        return E_FAIL;

    return S_OK;
}

HRESULT CImGuiMgr::Delete_HeightMap()
{
    if (m_pTerrain == nullptr)
        return E_FAIL;

    if (FAILED(m_pTerrain->Delete_DynamicBuffer()))
        return E_FAIL;

    return S_OK;
}

HRESULT CImGuiMgr::Set_Control_Variable(void* pArg)
{
    if (m_pTerrain == nullptr)
        return E_FAIL;

    if (FAILED(m_pTerrain->Set_Control_Variable(pArg)))
        return E_FAIL;

    return S_OK;
}

void CImGuiMgr::Set_Terrain_Edit()
{
   
    IMGUIMGRWINDESC* ImguiMrgWinDesc = new IMGUIMGRWINDESC;

    ImguiMrgWinDesc->strName = "Terrain";
    ImguiMrgWinDesc->window_flags = ImGuiWindowFlags_HorizontalScrollbar
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    ImguiMrgWinDesc->vWinSize = ImVec2(300, 500);

    m_vecWindow.push_back(CTerrain_Window::Create(ImguiMrgWinDesc));
}

void CImGuiMgr::Free()
{
    Safe_Release(m_pTerrain);

    for (auto& iter : m_vecWindow)
        Safe_Release(iter);
    m_vecWindow.clear();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	__super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}

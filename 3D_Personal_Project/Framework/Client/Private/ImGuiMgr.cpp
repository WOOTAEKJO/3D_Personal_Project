#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"

#include "../Public/ImGuiMgr.h"

#include "commdlg.h"
#include "shlwapi.h"

#include "GameInstance.h"

#include "Terrain_Window.h"
#include "Camera_Window.h"
#include "Object_Window.h"


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

    if(FAILED(Ready_Demo()))
        return E_FAIL;

   Set_Terrain_Edit();
   Set_Object_Edit();
   Set_Camera_Edit();

	return S_OK;
}

void CImGuiMgr::Tick()
{
    Update_Terrain_Pick();
    Update_Demo_Pick();

    for (auto& iter : m_mapWindow[M_eCurentMode]) {
        if (iter.second != nullptr) {
            iter.second->Tick();
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

    ImGuizmo::BeginFrame();

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

            if (ImGui::MenuItem("Load"))
            {
            }

            ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("Mode"))
        {
            if (ImGui::MenuItem("HeightMap"))
            {
                M_eCurentMode = MODE_TERRAIN;
            }

            if (ImGui::MenuItem("Object"))
            {
                M_eCurentMode = MODE_OBJECT;
            }

            if (ImGui::MenuItem("Camera"))
            {
                M_eCurentMode = MODE_CAMERA;
            }

            if (ImGui::MenuItem("UI"))
            {
            }

            if (ImGui::MenuItem("Effect"))
            {
            }

            ImGui::EndMenu();
        }

		ImGui::EndMainMenuBar();
	}

    for (auto& iter : m_mapWindow[M_eCurentMode])
    {
        if (iter.second != nullptr)
            iter.second->Render();
    }

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

void CImGuiMgr::Window_Set_Variable(IMGUIMODE eType, WINDOWSTATE eWindowTag, void* pArg)
{
    CImGui_Window* pWindow = Find_Window(eType, eWindowTag);
    if (pWindow == nullptr)
        return;

    pWindow->Set_Variable(pArg);
}

void CImGuiMgr::Set_Terrain_Edit()
{
    IMGUIMGRWINDESC* ImguiMrgWinDesc = new IMGUIMGRWINDESC;

    ImguiMrgWinDesc->strName = "Terrain";
    ImguiMrgWinDesc->window_flags = ImGuiWindowFlags_HorizontalScrollbar
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    ImguiMrgWinDesc->vWinSize = ImVec2(300, 500);

    m_mapWindow[MODE_TERRAIN].emplace(WS_MAIN, CTerrain_Window::Create(ImguiMrgWinDesc));
}

void CImGuiMgr::Set_Object_Edit()
{
    IMGUIMGRWINDESC* ImguiMrgWinDesc = new IMGUIMGRWINDESC;

    ImguiMrgWinDesc->strName = "Object";
    ImguiMrgWinDesc->window_flags = ImGuiWindowFlags_HorizontalScrollbar
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    ImguiMrgWinDesc->vWinSize = ImVec2(300, 500);

    m_mapWindow[MODE_OBJECT].emplace(WS_MAIN, CObject_Window::Create(ImguiMrgWinDesc));
}

void CImGuiMgr::Set_Camera_Edit()
{
    IMGUIMGRWINDESC* ImguiMrgWinDesc = new IMGUIMGRWINDESC;

    ImguiMrgWinDesc->strName = "Camera";
    ImguiMrgWinDesc->window_flags = ImGuiWindowFlags_HorizontalScrollbar
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    ImguiMrgWinDesc->vWinSize = ImVec2(300, 500);

    m_mapWindow[MODE_CAMERA].emplace(WS_MAIN, CCamera_Window::Create(ImguiMrgWinDesc));
}

void CImGuiMgr::Update_Terrain_Pick()
{
    if (m_pTerrain == nullptr)
        return;

    if (m_pTerrain->Update_Mouse(&m_vPickedPoint))
    {
        CImGui_Window* pWindow = Find_Window(M_eCurentMode, WS_MAIN);
        if (pWindow == nullptr)
            return;

        if (m_pGameInstance->Key_Pressing(DIK_SPACE)) {
            pWindow->Terrain_Picked(m_vPickedPoint);
        }
    }
}

void CImGuiMgr::Update_Demo_Pick()
{
    if (m_mapWindow[M_eCurentMode].empty())
        return;

    for (auto& iter : m_mapWindow[M_eCurentMode]) {
        if (iter.second != nullptr)
            iter.second->Demo_Picked();
    }
    
}

HRESULT CImGuiMgr::Ready_Demo()
{
    if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, TEXT("Tool"),
        TEXT("Prototype_GameObject_Terrain_Demo"), nullptr, reinterpret_cast<CGameObject**>(&m_pTerrain))))
        return E_FAIL;

    return S_OK;
}

CImGui_Window* CImGuiMgr::Find_Window(IMGUIMODE eType, WINDOWSTATE eWindowTag)
{
    auto& iter = m_mapWindow[eType].find(eWindowTag);
    if (iter == m_mapWindow[eType].end())
        return nullptr;

    return iter->second;
}

void CImGuiMgr::Free()
{
    Safe_Release(m_pTerrain);

    for (_uint i = 0; i < MODE_END; i++) {
        for (auto& iter : m_mapWindow[i])
            Safe_Release(iter.second);
        m_mapWindow[i].clear();
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	__super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}

#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Imgui/ImGuiFileDialog-master/ImGuiFileDialogConfig.h"
#include "../Imgui/ImGuiFileDialog-master/ImGuiFileDialog.h"

#include "../Public/ImGuiMgr.h"

#include "commdlg.h"
#include "shlwapi.h"
#include <fstream>

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

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	return S_OK;
}

void CImGuiMgr::Tick()
{
    
}

HRESULT CImGuiMgr::Render()
{
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
            m_bFileDialog = true;

            ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("Mode"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Etc"))
        {

            ImGui::EndMenu();
        }

		ImGui::EndMainMenuBar();
	}


    if (m_bFileDialog)
    {
        File_Render();
    }
        
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

void CImGuiMgr::File_Render()
{
    const char* Path = nullptr;
    const char* Filter = nullptr;
   
    ImGuiFileDialog::Instance()->OpenDialog("FileDialog", "Choose File", ".bin", DATA_TERRAIN_PATH);

    // display
    if (ImGuiFileDialog::Instance()->Display("FileDialog"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string strFileName = ImGuiFileDialog::Instance()->GetFilePathName();
            string strFilePath = ImGuiFileDialog::Instance()->GetCurrentPath();


        }

        // close
        ImGuiFileDialog::Instance()->Close();
        m_bFileDialog = false;
    }
    // 파일 다이얼로그만 키면 누수가 남
}

HRESULT CImGuiMgr::Binarization()
{
    return S_OK;
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

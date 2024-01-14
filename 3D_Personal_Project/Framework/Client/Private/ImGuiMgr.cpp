#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Imgui/ImGuiFileDialog/ImGuiFileDialogConfig.h"
#include "../Imgui/ImGuiFileDialog/ImGuiFileDialog.h"

#include "../Public/ImGuiMgr.h"

#include "commdlg.h"
#include "shlwapi.h"
#include <fstream>

#include "GameInstance.h"

#include "Terrain_Window.h"
#include "Camera_Window.h"
#include "Object_Window.h"
#include "Animation_Window.h"

#include "Terrain_Demo.h"

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

    if(FAILED(Ready_Demo()))
        return E_FAIL;

   Set_Object_Edit();
   Set_Terrain_Edit();
   Set_Camera_Edit();
   Set_Animation_Edit();

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
                m_eFileMode = FILEMODE_SAVE;
            }

            if (ImGui::MenuItem("Load"))
            {
                m_eFileMode = FILEMODE_LOAD;
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

            if (ImGui::MenuItem("Animation"))
            {
                M_eCurentMode = MODE_ANIMATION;
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

        if (ImGui::BeginMenu("Etc"))
        {
            if (ImGui::MenuItem("Grid"))
            {
                if (m_bGrid)
                    m_bGrid = false;
                else
                    m_bGrid = true;
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

    if (m_eFileMode != FILEMODE_END)
        File_Render();
  
    if (m_bGrid)
        Grid_Draw();

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

vector<CObjectMesh_Demo*>* CImGuiMgr::Get_ObjectDemo()
{
    CImGui_Window* pWindow = Find_Window(IMGUIMODE::MODE_OBJECT, WINDOWSTATE::WS_MAIN);
    if (pWindow == nullptr)
        return nullptr;

    return dynamic_cast<CObject_Window*>(pWindow)->Get_ObjectDemo();
}

void CImGuiMgr::Set_Terrain_Edit()
{
    CImGui_Window::IMGUIWINDESC ImguiMrgWinDesc;

    ImguiMrgWinDesc.strName = "Terrain";
    ImguiMrgWinDesc.window_flags = CImGui_Window::WINDOWFLAGS::HorizontalScrollbar
        | CImGui_Window::WINDOWFLAGS::NoMove | CImGui_Window::WINDOWFLAGS::NoResize;
    ImguiMrgWinDesc.vWinSize = _float2(300.f, 500.f);

    m_mapWindow[MODE_TERRAIN].emplace(WS_MAIN, CTerrain_Window::Create(m_pDevice,m_pContext,&ImguiMrgWinDesc));
}

void CImGuiMgr::Set_Object_Edit()
{
    CImGui_Window::IMGUIWINDESC ImguiMrgWinDesc;

    ImguiMrgWinDesc.strName = "Object";
    ImguiMrgWinDesc.window_flags = CImGui_Window::WINDOWFLAGS::HorizontalScrollbar
        | CImGui_Window::WINDOWFLAGS::NoMove | CImGui_Window::WINDOWFLAGS::NoResize;
    ImguiMrgWinDesc.vWinSize = _float2(300.f, 500.f);

    m_mapWindow[MODE_OBJECT].emplace(WS_MAIN, CObject_Window::Create(m_pDevice, m_pContext, &ImguiMrgWinDesc));
}

void CImGuiMgr::Set_Camera_Edit()
{
    CImGui_Window::IMGUIWINDESC ImguiMrgWinDesc;

    ImguiMrgWinDesc.strName = "Camera";
    ImguiMrgWinDesc.window_flags = CImGui_Window::WINDOWFLAGS::HorizontalScrollbar
        | CImGui_Window::WINDOWFLAGS::NoMove | CImGui_Window::WINDOWFLAGS::NoResize;
    ImguiMrgWinDesc.vWinSize = _float2(300.f, 500.f);

    m_mapWindow[MODE_CAMERA].emplace(WS_MAIN, CCamera_Window::Create(m_pDevice, m_pContext, &ImguiMrgWinDesc));
}

void CImGuiMgr::Set_Animation_Edit()
{
    CImGui_Window::IMGUIWINDESC ImguiMrgWinDesc;

    ImguiMrgWinDesc.strName = "Animation";
    ImguiMrgWinDesc.window_flags = CImGui_Window::WINDOWFLAGS::HorizontalScrollbar
        | CImGui_Window::WINDOWFLAGS::NoMove | CImGui_Window::WINDOWFLAGS::NoResize;
    ImguiMrgWinDesc.vWinSize = _float2(350.f, 700.f);

    m_mapWindow[MODE_ANIMATION].emplace(WS_MAIN, CAnimation_Window::Create(m_pDevice, m_pContext, &ImguiMrgWinDesc));
}

void CImGuiMgr::Update_Terrain_Pick()
{
    if (m_pTerrain == nullptr)
        return;

    CImGui_Window* pWindow = Find_Window(M_eCurentMode, WS_MAIN);
    if (pWindow == nullptr)
        return;

    if (m_pTerrain->Update_Mouse(&m_vPickedPoint, pWindow->Is_Pressing()))
    {
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

HRESULT CImGuiMgr::Save_Data(const _char* strFilePath)
{
    CImGui_Window* pWindow = Find_Window(M_eCurentMode, WINDOWSTATE::WS_MAIN);
    if (pWindow == nullptr)
        return E_FAIL;

    return pWindow->Save_Data(strFilePath);
}

HRESULT CImGuiMgr::Load_Data(const _char* strFilePath)
{
    CImGui_Window* pWindow = Find_Window(M_eCurentMode, WINDOWSTATE::WS_MAIN);
    if (pWindow == nullptr)
        return E_FAIL;

    return pWindow->Load_Data(strFilePath);
}

void CImGuiMgr::Grid_Draw()
{
    //_float4x4 matView = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW);
    //_float arrView[] = { matView._11,matView._12,matView._13,matView._14,
    //                matView._21,matView._22,matView._23,matView._24,
    //                matView._31,matView._32,matView._33,matView._34,
    //                matView._41,matView._42,matView._43,matView._44 };

    //_float4x4 matProj = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ);
    //_float arrProj[] = { matProj._11,matProj._12,matProj._13,matProj._14,
    //                matProj._21,matProj._22,matProj._23,matProj._24,
    //                matProj._31,matProj._32,matProj._33,matProj._34,
    //                matProj._41,matProj._42,matProj._43,matProj._44 };

    ///*_float arrIdentity[] = { 1.f,0.f,0.f,0.f,
    //                0.f,1.f,0.f,0.f,
    //               0.f,0.f,1.f,0.f,
    //                0.f,0.f,0.f,1.f };*/

    //_float4x4 matWorld = m_pTerrain->Get_WorldMatrix();
    //_float arrWorld[] = { matWorld._11,matWorld._12,matWorld._13,matWorld._14,
    //                 matWorld._21,matWorld._22,matWorld._23,matWorld._24,
    //                 matWorld._31,matWorld._32,matWorld._33,matWorld._34,
    //                 matWorld._41,matWorld._42,matWorld._43,matWorld._44 };

    //ImGuizmo::DrawGrid(arrView, arrProj, arrWorld, 100.f);
}

void CImGuiMgr::File_Render()
{
   
    const char* Filter = nullptr;

    CImGui_Window* pWindow = Find_Window(M_eCurentMode, WINDOWSTATE::WS_MAIN);
    string  strPath = pWindow->Get_Path();
    switch (M_eCurentMode)
    {
    case Client::CImGuiMgr::MODE_STATIC:
        return;
    case Client::CImGuiMgr::MODE_TERRAIN:
        Filter = ".bin";
        break;
    case Client::CImGuiMgr::MODE_OBJECT:
        Filter = ".bin";
        break;
    case Client::CImGuiMgr::MODE_ANIMATION:
        Filter = ".json";
        break;
    case Client::CImGuiMgr::MODE_CAMERA:
        break;
    }

    ImGuiFileDialog::Instance()->OpenDialog("FileDialog", "Choose File", Filter, strPath.c_str());

    // display
    if (ImGuiFileDialog::Instance()->Display("FileDialog"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string strFileName = ImGuiFileDialog::Instance()->GetFilePathName();
            string strFilePath = ImGuiFileDialog::Instance()->GetCurrentPath();

            if (m_eFileMode == FILEMODE::FILEMODE_SAVE)
                Save_Data(strFileName.c_str());
            else if (m_eFileMode == FILEMODE::FILEMODE_LOAD)
                Load_Data(strFileName.c_str());
        }

        // close
        ImGuiFileDialog::Instance()->Close();
        m_eFileMode = FILEMODE_END;
    }
    // 파일 다이얼로그만 키면 누수가 남
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
   
    for (_uint i = 0; i < (_uint)MODE_END; i++) {
        for (auto& iter : m_mapWindow[i])
            Safe_Release(iter.second);
        m_mapWindow[i].clear();
    }

    Safe_Release(m_pTerrain);

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

	__super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}

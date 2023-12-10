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

#include <sstream>

#include "GameInstance.h"

#include "Mesh_Demo.h"

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

    if (FAILED(Init_Model()))
        return E_FAIL;

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

    if (ImGui::BeginTabBar("Mesh"))
    {
        if (ImGui::BeginTabItem("AnimMesh"))
        {
            m_eCurrentType = TYPE_ANIM;

            ImVec2 vSize = ImVec2(250, 100);
            ImGui::BeginListBox("AnimMesh", vSize);
            _uint iSize = m_vecMesh_Demo[TYPE_ANIM].size();
            for (_uint i = 0; i < iSize; i++)
            {
                string str = to_string(i);
                string str2;
                size_t pos = m_vecMesh_Demo[TYPE_ANIM][i]->Get_ModelTag().rfind(L"_");
                wstring wstr = m_vecMesh_Demo[TYPE_ANIM][i]->Get_ModelTag().substr(pos + 1);
                str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
                if (ImGui::Selectable((str2).c_str(), i == m_iCurrentIndex)) {
                    m_iCurrentIndex = i;
                    //m_strCurrentDemoTag = (str + "." + str2);
                }
            }
            ImGui::EndListBox();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("NonAnimMesh"))
        {
            m_eCurrentType = TYPE_NONANIM;

            ImVec2 vSize = ImVec2(250, 100);
            ImGui::BeginListBox("NonAnimMesh",vSize);
            _uint iSize = m_vecMesh_Demo[TYPE_NONANIM].size();
            for (_uint i = 0; i < iSize; i++)
            {
                string str = to_string(i);
                string str2;
                size_t pos = m_vecMesh_Demo[TYPE_NONANIM][i]->Get_ModelTag().rfind(L"_");
                wstring wstr = m_vecMesh_Demo[TYPE_NONANIM][i]->Get_ModelTag().substr(pos + 1);
                str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
                if (ImGui::Selectable((str2).c_str(), i == m_iCurrentIndex)) {
                    m_iCurrentIndex = i;
                    //m_strCurrentDemoTag = (str + "." + str2);
                }
            }
            ImGui::EndListBox();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
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

            Binarization(strFileName.c_str());
        }

        // close
        ImGuiFileDialog::Instance()->Close();
        m_bFileDialog = false;
    }
    // 파일 다이얼로그만 키면 누수가 남
}

HRESULT CImGuiMgr::Binarization(const _char* strFilePath)
{
    if (FAILED(m_vecMesh_Demo[m_eCurrentType][m_iCurrentIndex]->Binarization(strFilePath)))
        return E_FAIL;

    return S_OK;
}

HRESULT CImGuiMgr::Init_Model()
{
    CComponent_Manager::PROTOTYPE mapProtoCom = m_pGameInstance->Get_Com_ProtoType(m_pGameInstance->Get_Current_Level());

    _uint pos;
    wstring wstr;
    for (auto& iter : mapProtoCom)
    {
        wstr = iter.first;
        for (_uint i = 0; i < 2; i++) {
            pos = wstr.find(L"_");
            wstr = wstr.substr(pos + 1);
        }
        wstr = Split_Wstring(wstr, L'_');

        if (!wcscmp(wstr.c_str(), L"AnimModel"))
        {
            if (FAILED(Create_Model(TYPE_ANIM, iter.first)))
                return E_FAIL;
        }
        else if (!wcscmp(wstr.c_str(), L"Model"))
        {
            if (FAILED(Create_Model(TYPE_NONANIM, iter.first)))
                return E_FAIL;
        }

    }

    return S_OK;
}

HRESULT CImGuiMgr::Create_Model(MODELTYPE eType, const wstring& strTag)
{
    CGameObject* pDemo = nullptr;

    CMesh_Demo::OBDEMOVALUE DemoValue;

    DemoValue.strModelTag = strTag;

    if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), TEXT("Converter"), G0_MESH_DEMO_TAG, &DemoValue,
        reinterpret_cast<CGameObject**>(&pDemo))))
        return E_FAIL;

    m_vecMesh_Demo[eType].push_back(dynamic_cast<CMesh_Demo*>(pDemo));

    return S_OK;
}

wstring CImGuiMgr::Split_Wstring(wstring strFull, _tchar cSeperator)
{
    wstring wstr;
    wstringstream wss(strFull);
    getline(wss, wstr, cSeperator);

    return wstr;
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

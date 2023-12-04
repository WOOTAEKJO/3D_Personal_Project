#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Public/Terrain_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"
#include "Terrain_Demo.h"
#include "ObjectMesh_Demo.h"

CTerrain_Window::CTerrain_Window()
{
}

HRESULT CTerrain_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	wcscpy_s(m_szFile, DATA_TERRAIN_PATH);

	return S_OK;
}

void CTerrain_Window::Tick()
{
	Terrain_Update();
}

HRESULT CTerrain_Window::Render()
{
	CImGui_Window::Begin();

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Terrain", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("HeightMap"))
		{
			HeightMap();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Tile"))
		{


			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	CImGui_Window::End();

	m_szFile;

	return S_OK;
}

void CTerrain_Window::Set_Variable(void* pArg)
{
	if (pArg == nullptr)
		return;
}

void CTerrain_Window::Terrain_Picked(_float4 vPickPoint)
{
	if (m_pTerrain == nullptr)
		return;
	
	m_pTerrain->Update_HeightMap(XMLoadFloat4(&vPickPoint), (_float)m_iHeight_Control[0], (_float)m_iHeight_Control[1], m_fSharpness);

	m_vPickPos = vPickPoint;
}

void CTerrain_Window::Demo_Picked()
{
}

HRESULT CTerrain_Window::Save_Data()
{
	Set_File_Flag(TYPE::TYPE_SAVE);

	if (GetSaveFileName(&m_ofn) == TRUE)
	{
		Document doc;
		//json 객체 선언
		doc.SetObject();

		// json 구조체생성
		Value ValueOne(kObjectType);

		//json구조체에 저장할 구조체의 멤버 변환해서 저장
		ValueOne.AddMember("int", m_Data.m_iTest, doc.GetAllocator());

		ValueOne.AddMember("str", Value(wstring_To_Json(m_Data.m_strTest).c_str(), doc.GetAllocator()), doc.GetAllocator());

		//새로운 json 구조체 생성
		Value ValueTwo(kObjectType);
		//json 구조체에 값 저장
		ValueTwo.AddMember("str2", Value(wstring_To_Json(m_Data.m_strTest2).c_str(), doc.GetAllocator()), doc.GetAllocator());

		//json구조체들을 json 문서에 저장
		//1.저장할 구조체 이름, 2.구조체,3.객체의 할당기
		doc.AddMember("One", ValueOne, doc.GetAllocator());
		doc.AddMember("Two", ValueTwo, doc.GetAllocator());

		// json 문서를 문자열로 변환
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		doc.Accept(writer);

		ofstream ofs(m_ofn.lpstrFile);
		ofs << buffer.GetString();
		ofs.close();
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Window::Load_Data()
{
	Set_File_Flag(TYPE::TYPE_LOAD);

	if (GetOpenFileName(&m_ofn) == TRUE)
	{
		FILE* pFile = _tfopen(m_ofn.lpstrFile, _T("rb"));
		if (!pFile)
			return E_FAIL;

		fseek(pFile, 0, SEEK_END);
		size_t fileSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		vector<char> buffer(fileSize + 1, 0);
		fread(buffer.data(), 1, fileSize, pFile);
		fclose(pFile);

		Document document;
		document.Parse(buffer.data());

		if (document.HasParseError())
		{
			return MSG_BOX("불러오기 실패");
		}

		// json Document 찾기
		if (document.HasMember("One") && document["One"].IsObject())
		{
			//Document 의 value값
			const Value& dataValue = document["One"];

			//Value의 key값
			if (dataValue.HasMember("int") && dataValue["int"].IsInt())
				m_Data.m_iTest = dataValue["int"].GetInt();

			if (dataValue.HasMember("str") && dataValue["str"].IsString())
			{
				const char* strValue = dataValue["str"].GetString();
				m_Data.m_strTest = Json_To_wstring(strValue);
			}

		}
		if (document.HasMember("Two") && document["Two"].IsObject())
		{
			const Value& dataValue = document["Two"];

			if (dataValue.HasMember("str2") && dataValue["str2"].IsString())
			{
				const char* strValue = dataValue["str2"].GetString();
				m_Data.m_strTest2 = Json_To_wstring(strValue);
			}
		}

	}
	
	return S_OK;
}

void CTerrain_Window::HeightMap()
{
	ImGui::InputInt2("Vertices_SizeX,Z", m_iVertices_Size);

	if (ImGui::Button("Create_HeightMap"))
	{
		if (m_iVertices_Size[0] < 2 || m_iVertices_Size[1] < 2) {
			MSG_BOX("The number is too small");
			return;
		}

		Create_HeightMap();
	}

	ImGui::SliderInt("Radius", &m_iHeight_Control[0], 0, 30);
	ImGui::SliderInt("Height", &m_iHeight_Control[1], 0, 30);
	ImGui::SliderFloat("Sharpness", &m_fSharpness, 0.f, 1.f);

	ImGui::Checkbox("WireFrame", &m_bWireFrame);

	ImGui::Text(to_string(m_vPickPos.x).c_str());
}

void CTerrain_Window::Create_HeightMap()
{
	if (m_pTerrain == nullptr)
		        return;
		
	if (FAILED((m_pTerrain->Create_DynamicBuffer(m_iVertices_Size[0], m_iVertices_Size[1]))))
	    return ;
}

void CTerrain_Window::Terrain_Update()
{
	CTerrain_Demo::TERRAINDEMOVALUE tTerrainDemoValue;

	tTerrainDemoValue.fRadius = (_float)m_iHeight_Control[0];
	tTerrainDemoValue.bWireFrame = m_bWireFrame;

	m_pTerrain->Set_Control_Variable(&tTerrainDemoValue);
}

CTerrain_Window* CTerrain_Window::Create(void* pArg)
{
	CTerrain_Window* pInstance = new CTerrain_Window();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTerrain_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain_Window::Free()
{
	__super::Free();
}

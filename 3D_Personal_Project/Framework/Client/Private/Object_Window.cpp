#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "..\Public\Object_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"

#include "Terrain_Demo.h"
#include "ObjectMesh_Demo.h"
#include "AnimMesh_Demo.h"
#include "Demo.h"

CObject_Window::CObject_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CImGui_Window(pDevice, pContext)
{
}

HRESULT CObject_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

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

		if (!wcscmp(wstr.c_str(),L"Model"))
		{
			m_vecModelTag.push_back(iter.first);
		}
		else if (!wcscmp(wstr.c_str(), L"ModelInstancing"))
		{
			m_eInstancingModel.vecModelTag.push_back(iter.first);
		}
		else if (!wcscmp(wstr.c_str(), L"AnimModel"))
		{
			m_vecAnimModelTag.push_back(iter.first);
		}
	}

	for (auto& Demo : m_vecModelTag)
	{
		for (auto& Instance : m_eInstancingModel.vecModelTag)
		{
			if (!wcscmp(CUtility_String::Get_LastName(Instance).c_str(), CUtility_String::Get_LastName(Demo).c_str()))
			{
				m_eTMP.vecModelTag.push_back(Demo);
			}
		}
	}

	return S_OK;
}

void CObject_Window::Tick()
{
}

HRESULT CObject_Window::Render()
{
	CImGui_Window::Begin();

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Object", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("ObjectMesh"))
		{
			m_eCurrentType = TYPE::TYPE_NONANIM;
			ObjectMesh();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("AnimObjectMesh"))
		{
			m_eCurrentType = TYPE::TYPE_ANIM;
			AnimObjectMesh();
			ImGui::EndTabItem();
		}


		if (ImGui::BeginTabItem("Instancing"))
		{
			m_eCurrentType = TYPE::TYPE_INSTANCING;
			InstancingMesh();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	CImGui_Window::End();

	return S_OK;
}

void CObject_Window::Set_Variable(void* pArg)
{
	if (pArg == nullptr)
		return;
}

void CObject_Window::Terrain_Picked(_float4 vPickPoint)
{
	if (m_pTerrain == nullptr)
		return;
	if (m_eCurrentType == TYPE::TYPE_NONANIM)
		Create_Model(g_strLayerName[m_iCurrentLayerName], m_strPickModelTag, vPickPoint);
	else if (m_eCurrentType == TYPE::TYPE_ANIM)
		Create_AnimModel(g_strLayerName[m_iCurrentLayerName], m_strPickAnimModelTag, vPickPoint);
	else if (m_eCurrentType == TYPE::TYPE_INSTANCING)
		Create_TMP(g_strLayerName[m_iCurrentLayerName], m_eTMP.strPickModelTag, vPickPoint);
}

void CObject_Window::Demo_Picked()
{
	if (m_vecDemo.empty())
		return;

	_float4 vOutPos;

 	if (m_pGameInstance->Mouse_Down(DIM_RB)) {

		if (m_eCurrentType == TYPE::TYPE_NONANIM) {

			_uint iSize = m_vecDemo.size();
			_float fMinDist = 1000.f;
			_float fDist = 0.f;

			for (size_t i = 0; i < iSize; i++)
			{
				if (m_vecDemo[i]->Get_Picked_Dist(&vOutPos, &fDist))
				{
					if (fMinDist > fDist)
					{
						fMinDist = fDist;
						m_iCurrentDemoIndex = (_uint)i;
					}
				}
			}
		}
		else if (m_eCurrentType == TYPE::TYPE_ANIM)
		{
			for (size_t i = 0; i < m_vecAnimDemo.size(); i++)
			{
				if (m_vecAnimDemo[i]->Get_Picked())
				{
					m_iCurrentAnimDemoIndex = (_uint)i;
					return;
				}
			}
		}
	}
}

string CObject_Window::Get_Path()
{
	if (m_eCurrentType == TYPE::TYPE_INSTANCING)
	{
		return "../Bin/Data/Object/Instancing/";
	}
	else {
		return "../Bin/Data/Object/";
	}
	
}

HRESULT CObject_Window::Save_Data(const _char* strFilePath)
{
	ofstream fout;
	_bool bAnim = false;

	fout.open(strFilePath, std::ofstream::binary);

	if (fout.is_open())
	{
		if (m_eCurrentType == TYPE::TYPE_INSTANCING)
		{
			_uint iObjNum = m_eInstancingModel.vecDemo.size();
			fout.write(reinterpret_cast<const char*>(&iObjNum), sizeof(_uint));

			for (auto& iter : m_eInstancingModel.vecDemo)
			{
				string strLayer = CUtility_String::WString_To_string(iter->Get_LayerName());
				size_t istrSize = strLayer.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strLayer.c_str(), istrSize);

				string strProTag = CUtility_String::WString_To_string(GO_PLATEFORM_INSTANCING_TAG);
				istrSize = strProTag.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strProTag.c_str(), istrSize);

				string strModelTag = CUtility_String::WString_To_string(iter->Get_ModelTag());
				istrSize = strModelTag.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strModelTag.c_str(), istrSize);

				//_int iNaviInd = -1;
				_int iNaviInd = iter->Get_NaviCellIndex();
				fout.write(reinterpret_cast<const char*>(&iNaviInd), sizeof(_int));

				fout.write(reinterpret_cast<const char*>(&iter->Get_Component<CTransform>()->Get_WorldMatrix_Float4x4())
					, sizeof(_float4x4));


				// 인스턴스 정점 상태 행렬 정보들

				vector<_float4x4> vecVertex = iter->Get_Component<CModel_Instancing>()->Get_InstanceVertex();
				_uint ivecVertexSize = vecVertex.size();
				fout.write(reinterpret_cast<const char*>(&ivecVertexSize), sizeof(_uint));

				for (auto& InstanceVertex : vecVertex)
				{
					fout.write(reinterpret_cast<const char*>(&InstanceVertex), sizeof(_float4x4));
				}
			}
		}
		else
		{
			_uint iObjNum = m_vecDemo.size() + m_vecAnimDemo.size();
			fout.write(reinterpret_cast<const char*>(&iObjNum), sizeof(_uint));

			for (auto& iter : m_vecDemo)
			{
				fout.write(reinterpret_cast<const char*>(&bAnim), sizeof(_bool));

				string strLayer = CUtility_String::WString_To_string(iter->Get_LayerName());
				size_t istrSize = strLayer.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strLayer.c_str(), istrSize);

				string strProTag;
				
				_int iNaviInd = iter->Get_NaviCellIndex();
				if (iNaviInd == -2)
					strProTag = CUtility_String::WString_To_string(GO_PLATEFORM_TRAP_TAG);
				else
					strProTag = CUtility_String::WString_To_string(GO_PLATEFORM_TAG);

				istrSize = strProTag.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strProTag.c_str(), istrSize);

				string strModelTag = CUtility_String::WString_To_string(iter->Get_ModelTag());
				istrSize = strModelTag.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strModelTag.c_str(), istrSize);

				//_int iNaviInd = -1;
				fout.write(reinterpret_cast<const char*>(&iNaviInd), sizeof(_int));

				fout.write(reinterpret_cast<const char*>(&iter->Get_Component<CTransform>()->Get_WorldMatrix_Float4x4())
					, sizeof(_float4x4));
			}

			for (auto& iter : m_vecAnimDemo)
			{
				bAnim = true;
				fout.write(reinterpret_cast<const char*>(&bAnim), sizeof(_bool));

				string strLayer = CUtility_String::WString_To_string(iter->Get_LayerName());
				size_t istrSize = strLayer.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strLayer.c_str(), istrSize);

				string strProTag = CUtility_String::WString_To_string(iter->Get_ModelTag());
				istrSize = strProTag.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strProTag.c_str(), istrSize);

				string strModelTag = CUtility_String::WString_To_string(iter->Get_ModelTag());
				istrSize = strModelTag.size();
				fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
				fout.write(strModelTag.c_str(), istrSize);

				_int iNaviInd = iter->Get_NaviCellIndex();
				fout.write(reinterpret_cast<const char*>(&iNaviInd), sizeof(_int));

				fout.write(reinterpret_cast<const char*>(&iter->Get_Component<CTransform>()->Get_WorldMatrix_Float4x4())
					, sizeof(_float4x4));
			}
		}
	}
	else
		return E_FAIL;

	fout.close();

	return S_OK;
}

HRESULT CObject_Window::Load_Data(const _char* strFilePath)
{
	ifstream fIn;
	
	fIn.open(strFilePath, std::ios::binary);

	if (fIn.is_open())
	{
		if (m_eCurrentType == TYPE::TYPE_INSTANCING)
		{
			_uint iSize;
			fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));

			for (_uint i = 0; i < iSize; i++)
			{
				size_t LayerNameSize = {};
				fIn.read(reinterpret_cast<char*>(&LayerNameSize), sizeof(size_t));
				string strLayerName;
				strLayerName.resize(LayerNameSize);
				fIn.read(&strLayerName[0], LayerNameSize);

				size_t ProTagNameSize = {};
				fIn.read(reinterpret_cast<char*>(&ProTagNameSize), sizeof(size_t));
				string strProTagName;
				strProTagName.resize(ProTagNameSize);
				fIn.read(&strProTagName[0], ProTagNameSize);

				size_t ModelNameSize = {};
				fIn.read(reinterpret_cast<char*>(&ModelNameSize), sizeof(size_t));
				string strModelName;
				strModelName.resize(ModelNameSize);
				fIn.read(&strModelName[0], ModelNameSize);

				_int iNaviIndex;
				fIn.read(reinterpret_cast<char*>(&iNaviIndex), sizeof(_int));

				_float4x4 matWorld;
				fIn.read(reinterpret_cast<char*>(&matWorld), sizeof(_float4x4));

				_uint ivecVertexSize = 0;
				fIn.read(reinterpret_cast<char*>(&ivecVertexSize), sizeof(_uint));

				vector<_float4x4> vecVertex;
				vecVertex.reserve(ivecVertexSize);

				for (_uint j = 0; j < ivecVertexSize; j++)
				{
					_float4x4 matVertex;
					fIn.read(reinterpret_cast<char*>(&matVertex), sizeof(_float4x4));
					vecVertex.push_back(matVertex);
				}

				CGameObject* pObject_Demo = nullptr;
				CObjectMesh_Demo::OBDEMOVALUE ObjectDemoValue;

				ObjectDemoValue.strModelTag = CUtility_String::string_To_Wstring(strModelName);
				ObjectDemoValue.vPos = _float4(0.f, 0.f, 0.f, 1.f);
				ObjectDemoValue.vecVertexMat = vecVertex;

				strLayerName = CUtility_String::WString_To_string(g_strLayerName[LAYER::LAYER_PLATEFORM]);

				if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level()
					, CUtility_String::string_To_Wstring(strLayerName), G0_OBJECTMESH_DEMO_TAG,
					&ObjectDemoValue, reinterpret_cast<CGameObject**>(&pObject_Demo))))
					return E_FAIL;

				dynamic_cast<CObjectMesh_Demo*>(pObject_Demo)->Set_NaviCellIndex(iNaviIndex);

				m_eInstancingModel.vecDemo.push_back(dynamic_cast<CObjectMesh_Demo*>(pObject_Demo));

				pObject_Demo->Get_Component<CTransform>()->Set_WorldMatrix(matWorld);
			}
		}
		else
		{
			_uint iSize;
			fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));

			for (_uint i = 0; i < iSize; i++) {

				_bool bAnim;
				fIn.read(reinterpret_cast<char*>(&bAnim), sizeof(_bool));

				size_t LayerNameSize = {};
				fIn.read(reinterpret_cast<char*>(&LayerNameSize), sizeof(size_t));
				string strLayerName;
				strLayerName.resize(LayerNameSize);
				fIn.read(&strLayerName[0], LayerNameSize);

				size_t ProTagNameSize = {};
				fIn.read(reinterpret_cast<char*>(&ProTagNameSize), sizeof(size_t));
				string strProTagName;
				strProTagName.resize(ProTagNameSize);
				fIn.read(&strProTagName[0], ProTagNameSize);

				size_t ModelNameSize = {};
				fIn.read(reinterpret_cast<char*>(&ModelNameSize), sizeof(size_t));
				string strModelName;
				strModelName.resize(ModelNameSize);
				fIn.read(&strModelName[0], ModelNameSize);

				_int iNaviIndex;
				fIn.read(reinterpret_cast<char*>(&iNaviIndex), sizeof(_int));

				_float4x4 matWorld;
				fIn.read(reinterpret_cast<char*>(&matWorld), sizeof(_float4x4));

				CGameObject* pObject_Demo = nullptr;
				CDemo::Demo_Desc ObjectDemoValue;

				ObjectDemoValue.strModelTag = CUtility_String::string_To_Wstring(strModelName);
				ObjectDemoValue.vPos = _float4(0.f, 0.f, 0.f, 1.f);

				if (bAnim) {
					if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level()
						, CUtility_String::string_To_Wstring(strLayerName), G0_ANIMMESH_DEMO_TAG,
						&ObjectDemoValue, reinterpret_cast<CGameObject**>(&pObject_Demo))))
						return E_FAIL;

					dynamic_cast<CAnimMesh_Demo*>(pObject_Demo)->Set_NaviCellIndex(iNaviIndex);

					m_vecAnimDemo.push_back(dynamic_cast<CAnimMesh_Demo*>(pObject_Demo));
				}
				else {
					strLayerName = CUtility_String::WString_To_string(g_strLayerName[LAYER::LAYER_PLATEFORM]);

					if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level()
						, CUtility_String::string_To_Wstring(strLayerName), G0_OBJECTMESH_DEMO_TAG,
						&ObjectDemoValue, reinterpret_cast<CGameObject**>(&pObject_Demo))))
						return E_FAIL;

					dynamic_cast<CObjectMesh_Demo*>(pObject_Demo)->Set_NaviCellIndex(iNaviIndex);

					m_vecDemo.push_back(dynamic_cast<CObjectMesh_Demo*>(pObject_Demo));
				}

				pObject_Demo->Get_Component<CTransform>()->Set_WorldMatrix(matWorld);
			}
		}
	}
	else
		return E_FAIL;

	fIn.close();

	return S_OK;
}

void CObject_Window::Write_Json(json& Out_Json)
{
	if (m_eCurrentType == TYPE::TYPE_NONANIM) {
		_uint iSize = m_vecDemo.size();

		for (_uint i = 0; i < iSize; i++)
		{
			m_vecDemo[i]->Write_Json(Out_Json["Demo"][i]);
		}
	}
	else if (m_eCurrentType == TYPE::TYPE_ANIM)
	{
		_uint iSize = m_vecAnimDemo.size();

		for (_uint i = 0; i < iSize; i++)
		{
			m_vecAnimDemo[i]->Write_Json(Out_Json["AnimDemo"][i]);
		}
	}
}

void CObject_Window::Load_FromJson(const json& In_Json)
{
	for (auto& iter : m_vecDemo)
		iter->Set_Dead();
	m_vecDemo.clear();

	for (auto& iter : In_Json["Demo"])
	{
		CGameObject* pObject_Demo = nullptr;

		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, TEXT("Tool"), G0_OBJECTMESH_DEMO_TAG,
			nullptr, reinterpret_cast<CGameObject**>(&pObject_Demo))))
			return;

		pObject_Demo->Load_FromJson(iter);

		m_vecDemo.push_back(dynamic_cast<CObjectMesh_Demo*>(pObject_Demo));
	}
}

void CObject_Window::ObjectMesh()
{
	ImGui::Text("Object_List");
	ImVec2 vSize = ImVec2(250, 100);
	ImGui::BeginListBox("Model", vSize);
	for (auto& iter : m_vecModelTag) {

		size_t pos = iter.rfind(L"_");
		wstring wstr = iter.substr(pos + 1);
		string str;
		str.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable(str.c_str(), iter == m_strPickModelTag))
			m_strPickModelTag = iter;
	}
	ImGui::EndListBox();

	ImGui::BeginListBox("Demo", vSize);
	_uint iSize = m_vecDemo.size();
	for (_uint i = 0; i < iSize; i++)
	{
		string str = to_string(i);
		string str2;
		size_t pos = m_vecDemo[i]->Get_ModelTag().rfind(L"_");
		wstring wstr = m_vecDemo[i]->Get_ModelTag().substr(pos + 1);
		str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable((str + "." + str2).c_str(), i == m_iCurrentDemoIndex)) {
			m_iCurrentDemoIndex = i;
			m_strCurrentDemoTag = (str + "." + str2);
		}
	}
	ImGui::EndListBox();

	ImGui::BeginListBox("Layer", vSize);
	for (_uint i = 0; i < (_uint)LAYER::LAYER_END; i++)
	{
		string str = CUtility_String::WString_To_string(g_strLayerName[i]);
		if (ImGui::Selectable(str.c_str(), i == m_iCurrentLayerName)) {
			m_iCurrentLayerName = i;
		}
	}
	ImGui::EndListBox();

	

	if (!m_vecDemo.empty()&& m_vecDemo[m_iCurrentDemoIndex] != nullptr) {

		ImGui::Separator();
		string strMessage = "Selected : " + m_strCurrentDemoTag;
		ImGui::Text(strMessage.c_str());
		string strNaviInd = "NaviIndex : " + to_string(m_vecDemo[m_iCurrentDemoIndex]->Get_NaviCellIndex());
		ImGui::Text(strNaviInd.c_str());

		TransformGuizmo();

		__super::ImGuizmo(ImGuizmo::MODE::WORLD, m_vecDemo[m_iCurrentDemoIndex]);
	}

	ImGui::RadioButton("NonFunction", &m_iNaviIndxRadiButton, -1);
	ImGui::SameLine();
	ImGui::RadioButton("Trap", &m_iNaviIndxRadiButton, -2);

	if (ImGui::Button("Apply Function"))
	{
		m_vecDemo[m_iCurrentDemoIndex]->Set_NaviCellIndex(m_iNaviIndxRadiButton);
	}

	if (ImGui::Button("Delete Object")) {
		if (!m_vecDemo.empty() && m_vecDemo[m_iCurrentDemoIndex] != nullptr)
		{
			m_vecDemo[m_iCurrentDemoIndex]->Set_Dead();
			m_vecDemo.erase(m_vecDemo.begin() + m_iCurrentDemoIndex);

			if (m_iCurrentDemoIndex != 0)
				--m_iCurrentDemoIndex;
		}
	}
}

void CObject_Window::AnimObjectMesh()
{
	ImGui::Text("Object_List");
	ImVec2 vSize = ImVec2(250, 100);
	ImGui::BeginListBox("Model", vSize);
	for (auto& iter : m_vecAnimModelTag) {

		size_t pos = iter.rfind(L"_");
		wstring wstr = iter.substr(pos + 1);
		string str;
		str.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable(str.c_str(), iter == m_strPickAnimModelTag))
			m_strPickAnimModelTag = iter;
	}
	ImGui::EndListBox();

	ImGui::BeginListBox("Demo", vSize);
	_uint iSize = m_vecAnimDemo.size();
	for (_uint i = 0; i < iSize; i++)
	{
		string str = to_string(i);
		string str2;
		size_t pos = m_vecAnimDemo[i]->Get_ModelTag().rfind(L"_");
		wstring wstr = m_vecAnimDemo[i]->Get_ModelTag().substr(pos + 1);
		str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable((str + "." + str2).c_str(), i == m_iCurrentAnimDemoIndex)) {
			m_iCurrentAnimDemoIndex = i;
			m_strCurrentAnimDemoTag = (str + "." + str2);
		}
	}
	ImGui::EndListBox();

	ImGui::BeginListBox("Layer", vSize);
	for (_uint i = 0; i < (_uint)LAYER::LAYER_END; i++)
	{
		string str = CUtility_String::WString_To_string(g_strLayerName[i]);
		if (ImGui::Selectable(str.c_str(), i == m_iCurrentLayerName)) {
			m_iCurrentLayerName = i;
		}
	}
	ImGui::EndListBox();

	if (ImGui::Button("Delete AnimObject")) {
		if (!m_vecAnimDemo.empty() && m_vecAnimDemo[m_iCurrentAnimDemoIndex] != nullptr)
		{
			m_vecAnimDemo[m_iCurrentAnimDemoIndex]->Set_Dead();
			m_vecAnimDemo.erase(m_vecAnimDemo.begin() + m_iCurrentAnimDemoIndex);

			if (m_iCurrentAnimDemoIndex != 0)
				--m_iCurrentAnimDemoIndex;
		}
	}

	if (!m_vecAnimDemo.empty() && m_vecAnimDemo[m_iCurrentAnimDemoIndex] != nullptr) {

		ImGui::Separator();
		string strMessage = "Selected : " + m_strCurrentAnimDemoTag;
		ImGui::Text(strMessage.c_str());
		string strNaviInd = "NaviIndex : " + to_string(m_vecAnimDemo[m_iCurrentAnimDemoIndex]->Get_NaviCellIndex());
		ImGui::Text(strNaviInd.c_str());

		TransformGuizmo();

		__super::ImGuizmo(ImGuizmo::MODE::WORLD, m_vecAnimDemo[m_iCurrentAnimDemoIndex]);
	}
}

void CObject_Window::InstancingMesh()
{
	ImGui::Text("TMP_ModelList");
	ImVec2 vSize = ImVec2(200, 100);

	ImGui::BeginListBox("Model", vSize);
	for (auto& iter : m_eTMP.vecModelTag) {

		size_t pos = iter.rfind(L"_");
		wstring wstr = iter.substr(pos + 1);
		string str;
		str.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable(str.c_str(), iter == m_eTMP.strPickModelTag))
			m_eTMP.strPickModelTag = iter;
	}
	ImGui::EndListBox();

	ImGui::BeginListBox("TMP", vSize);
	_uint iSize = m_eTMP.vecDemo.size();
	for (_uint i = 0; i < iSize; i++)
	{
		string str = to_string(i);
		string str2;
		size_t pos = m_eTMP.vecDemo[i]->Get_ModelTag().rfind(L"_");
		wstring wstr = m_eTMP.vecDemo[i]->Get_ModelTag().substr(pos + 1);
		str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable((str + "." + str2).c_str(), i == m_eTMP.iCurrentIndex)) {
			m_eTMP.iCurrentIndex = i;
			m_eTMP.strCurrentTag = (str + "." + str2);
		}
	}
	ImGui::EndListBox();

	//ImGui::Text("Instancing_ModelList");
	//ImGui::BeginListBox("Model", vSize);
	//for (auto& iter : m_eInstancingModel.vecModelTag) {

	//	size_t pos = iter.rfind(L"_");
	//	wstring wstr = iter.substr(pos + 1);
	//	string str;
	//	str.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
	//	if (ImGui::Selectable(str.c_str(), iter == m_eInstancingModel.strPickModelTag))
	//		m_eInstancingModel.strPickModelTag = iter;
	//}
	//ImGui::EndListBox();

	ImGui::BeginListBox("Layer", vSize);
	for (_uint i = 0; i < (_uint)LAYER::LAYER_END; i++)
	{
		string str = CUtility_String::WString_To_string(g_strLayerName[i]);
		if (ImGui::Selectable(str.c_str(), i == m_iCurrentLayerName)) {
			m_iCurrentLayerName = i;
		}
	}
	ImGui::EndListBox();

	ImGui::Separator();
	string strMessage = "TMP Selected : " + m_eTMP.strCurrentTag;
	ImGui::Text(strMessage.c_str());

	if (!m_eTMP.vecDemo.empty() && m_eTMP.vecDemo[m_eTMP.iCurrentIndex] != nullptr) {

		TransformGuizmo();

		__super::ImGuizmo(ImGuizmo::MODE::WORLD, m_eTMP.vecDemo[m_eTMP.iCurrentIndex]);
	}

	if (ImGui::Button("TMP Delete Object")) {
		if (!m_eTMP.vecDemo.empty() && m_eTMP.vecDemo[m_eTMP.iCurrentIndex] != nullptr)
		{
			m_eTMP.vecDemo[m_eTMP.iCurrentIndex]->Set_Dead();
			m_eTMP.vecDemo.erase(m_eTMP.vecDemo.begin() + m_eTMP.iCurrentIndex);

			if (m_eTMP.iCurrentIndex != 0)
				--m_eTMP.iCurrentIndex;
		}
	}

	if (ImGui::Button("Instancing Create"))
	{
		Create_Instancing();
	}

	if (!m_eInstancingModel.vecDemo.empty())
	{
		ImGui::BeginListBox("Instancing", vSize);
		iSize = m_eInstancingModel.vecDemo.size();
		for (_uint i = 0; i < iSize; i++)
		{
			string str = to_string(i);
			string str2;
			size_t pos = m_eInstancingModel.vecDemo[i]->Get_ModelTag().rfind(L"_");
			wstring wstr = m_eInstancingModel.vecDemo[i]->Get_ModelTag().substr(pos + 1);
			str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
			if (ImGui::Selectable((str + "." + str2).c_str(), i == m_eInstancingModel.iCurrentIndex)) {
				m_eInstancingModel.iCurrentIndex = i;
				//m_eInstancingModel.strCurrentTag = (str + "." + str2);
			}
		}
		ImGui::EndListBox();
	}
}

void CObject_Window::Create_Model(const wstring& strLayerTag, const wstring& strModelTag, _float4 vPickPos)
{
	CGameObject* pObject_Demo = nullptr;
	CObjectMesh_Demo::OBDEMOVALUE ObjectDemoValue;

	ObjectDemoValue.strModelTag = strModelTag;
	ObjectDemoValue.vPos = vPickPos;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, strLayerTag, G0_OBJECTMESH_DEMO_TAG,
		&ObjectDemoValue,reinterpret_cast<CGameObject**>(&pObject_Demo))))
		return;

	m_vecDemo.push_back(dynamic_cast<CObjectMesh_Demo*>(pObject_Demo));
}

void CObject_Window::Create_AnimModel(const wstring& strLayerTag,const wstring& strModelTag, _float4 vPickPos)
{
	if (m_pTerrain == nullptr)
		return;

	CGameObject* pObject_Demo = nullptr;
	CAnimMesh_Demo::ANIMDEMOVALUE AnimDemoValue;

	AnimDemoValue.strModelTag = strModelTag;
	AnimDemoValue.vPos = vPickPos;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, strLayerTag, G0_ANIMMESH_DEMO_TAG,
		&AnimDemoValue, reinterpret_cast<CGameObject**>(&pObject_Demo))))
		return;

		dynamic_cast<CAnimMesh_Demo*>(pObject_Demo)->Set_NaviCellIndex(m_pTerrain->Get_Component<CNavigation>()
		->Find_PositionCell(XMLoadFloat4(&vPickPos)));
	
	m_vecAnimDemo.push_back(dynamic_cast<CAnimMesh_Demo*>(pObject_Demo));
}

void CObject_Window::Create_TMP(const wstring& strLayerTag, const wstring& strModelTag, _float4 vPickPos)
{
  	CGameObject* pObject_Demo = nullptr;
	CObjectMesh_Demo::OBDEMOVALUE ObjectDemoValue;

	ObjectDemoValue.strModelTag = strModelTag;
	ObjectDemoValue.vPos = vPickPos;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, strLayerTag, G0_OBJECTMESH_DEMO_TAG,
		&ObjectDemoValue, reinterpret_cast<CGameObject**>(&pObject_Demo))))
		return;

	m_eTMP.vecDemo.push_back(dynamic_cast<CObjectMesh_Demo*>(pObject_Demo));
}

void CObject_Window::Create_Instancing()
{
	CGameObject* pObject_Demo = nullptr;
	CObjectMesh_Demo::OBDEMOVALUE ObjectDemoValue;

	wstring strTag;

	for (auto& iter : m_eInstancingModel.vecModelTag)
	{
		if (!wcscmp(CUtility_String::Get_LastName(iter).c_str(), CUtility_String::Get_LastName(m_eTMP.strPickModelTag).c_str()))
		{
			strTag = iter;
			break;
		}
	}

	ObjectDemoValue.strModelTag = strTag;
	ObjectDemoValue.vPos = _float4(0.f,0.f,0.f,1.f);

	// 오브젝트들을 담아서 이제 만들어야 함
	// 구조가 이상해서 수정해야 함
	// 같은 오브젝트들을 다 찍고 그걸로 인스턴싱을 만들 생각이었음

	for (auto& iter : m_eTMP.vecDemo)
	{
		ObjectDemoValue.vecVertexMat.push_back(iter->Get_WorldMat());
	}

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, g_strLayerName[LAYER::LAYER_PLATEFORM], G0_OBJECTMESH_DEMO_TAG,
		&ObjectDemoValue, reinterpret_cast<CGameObject**>(&pObject_Demo))))
		return;

	m_eInstancingModel.vecDemo.push_back(dynamic_cast<CObjectMesh_Demo*>(pObject_Demo));

	for (auto& iter : m_eTMP.vecDemo)
	{
		iter->Set_Dead();
	}
	m_eTMP.vecDemo.clear();
}

void CObject_Window::NotGuizmo()
{
	if (!m_vecDemo.empty() && m_vecDemo[m_iCurrentDemoIndex] != nullptr) {

		ImGui::RadioButton("Pos", &m_iTransformRadioButton, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Rot", &m_iTransformRadioButton, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Scale", &m_iTransformRadioButton, 2);

		switch (m_iTransformRadioButton)
		{
		case 0:
			_float4 vPos;
			vPos = m_vecDemo[m_iCurrentDemoIndex]->Get_TransformState(CTransform::STATE::STATE_POS);
			m_fObjectPos[0] = vPos.x;
			m_fObjectPos[1] = vPos.y;
			m_fObjectPos[2] = vPos.z;

			ImGui::DragFloat3("X_Y_Z", m_fObjectPos, 1.f, 0.f, 10000.f);

			vPos = _float4(m_fObjectPos[0], m_fObjectPos[1], m_fObjectPos[2], 1.f);
			m_vecDemo[m_iCurrentDemoIndex]->Set_TransformState(CTransform::STATE::STATE_POS, vPos);

			break;
		case 1:
			ImGui::DragFloat3("X_Y_Z", m_fObjectRot, 1.f, -360.f, 360.f);

			m_vecDemo[m_iCurrentDemoIndex]->Rotation(m_fObjectRot[0], m_fObjectRot[1], m_fObjectRot[2]);
			break;
		case 2:
			ImGui::DragFloat3("X_Y_Z", m_fObjectScale, 0.1f, 0.01f, 100.f);

			m_vecDemo[m_iCurrentDemoIndex]->Set_Scale(m_fObjectScale[0], m_fObjectScale[1], m_fObjectScale[2]);
			break;
		default:
			break;
		}
		
	}
}

void CObject_Window::TransformGuizmo()
{
	ImGui::RadioButton("Pos", &m_iTransformRadioButton, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Rot", &m_iTransformRadioButton, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Scale", &m_iTransformRadioButton, 2);

	switch (m_iTransformRadioButton)
	{
	case 0:
		m_eOperationType = ImGuizmo::OPERATION::TRANSLATE;
		break;
	case 1:
		m_eOperationType = ImGuizmo::OPERATION::ROTATE;
		break;
	case 2:
		m_eOperationType = ImGuizmo::OPERATION::SCALE;
		break;
	default:
		break;
	}
	
}

CObject_Window* CObject_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg)
{
	CObject_Window* pInstance = new CObject_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CObject_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CObject_Window::Free()
{
	__super::Free();
}

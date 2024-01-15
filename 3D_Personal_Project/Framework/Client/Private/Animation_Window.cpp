#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "..\Public\Animation_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"

#include "Animation.h"
#include "Bone.h"

#include "AnimMesh_Demo.h"
#include "SubObject_Demo.h"

CAnimation_Window::CAnimation_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CImGui_Window(pDevice, pContext)
{
}

HRESULT CAnimation_Window::Initialize(void* pArg)
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

		if (!wcscmp(wstr.c_str(), L"AnimModel"))
		{
			m_vecAnimPrototypeTag.push_back(iter.first);
		}
		else if (!wcscmp(wstr.c_str(), L"Sub"))
		{
			m_vecSubPrototypeTag.push_back(iter.first);
		}

	}


	return S_OK;
}

void CAnimation_Window::Tick()
{
}

HRESULT CAnimation_Window::Render()
{
	CImGui_Window::Begin();

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Animation", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Animation"))
		{
			m_eCurrentMode = MODE_ANIMATION;
			Animation();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("SubObject"))
		{
			m_eCurrentMode = MODE_SUBOBJECT;
			SubObject();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	CImGui_Window::End();

	return S_OK;
}

void CAnimation_Window::Set_Variable(void* pArg)
{
}

void CAnimation_Window::Terrain_Picked(_float4 vPickPoint)
{
}

void CAnimation_Window::Demo_Picked()
{
}

string CAnimation_Window::Get_Path()
{
	return "../Bin/Data/Animation/";
}

HRESULT CAnimation_Window::Save_Data(const _char* strFilePath)
{
	json jSave;

	Write_Json(jSave);

	if (FAILED(CJson_Utility::Save_Json(strFilePath, jSave)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimation_Window::Load_Data(const _char* strFilePath)
{
	json jLoad;

	if (FAILED(CJson_Utility::Load_Json(strFilePath, jLoad)))
		return E_FAIL;

	Load_FromJson(jLoad);

	return S_OK;
}

void CAnimation_Window::Write_Json(json& Out_Json)
{
	switch (m_eCurrentMode)
	{
	case MODE_ANIMATION:

		if (m_pCurrentAnimModel == nullptr)
			return;

		m_pCurrentAnimModel->Write_Json(Out_Json);

		break;
	case MODE_SUBOBJECT:
		
		if (m_vecSubObjectDemo[m_iCurrentDemoIndex] == nullptr)
			return;
		Out_Json.emplace("BoneIndex", m_iCurrentAnimModelBoneIndex);
		m_vecSubObjectDemo[m_iCurrentDemoIndex]->Write_Json(Out_Json);
		
		break;
	}
}

void CAnimation_Window::Load_FromJson(const json& In_Json)
{
	switch (m_eCurrentMode)
	{
	case MODE_ANIMATION:
		
		break;
	case MODE_SUBOBJECT:

		if (m_vecSubObjectDemo[m_iCurrentDemoIndex] == nullptr)
			return;

		m_iCurrentAnimModelBoneIndex = In_Json["BoneIndex"];
		m_vecSubObjectDemo[m_iCurrentDemoIndex]->Load_FromJson(In_Json);
		m_vecSubObjectDemo[m_iCurrentDemoIndex]->
			Set_SocketBone(m_pCurrentAnimModel->Get_Bone(m_iCurrentAnimModelBoneIndex));

		break;
	}
}

void CAnimation_Window::Animation()
{
	ImGui::Text("AnimObject_List");
	ImVec2 vSize = ImVec2(250, 100);
	ImGui::BeginListBox("Anim_Proto", vSize);
	for (auto& iter : m_vecAnimPrototypeTag) {

		size_t pos = iter.rfind(L"_");
		wstring wstr = iter.substr(pos + 1);
		string str;
		str.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable(str.c_str(), iter == m_strPickModelTag))
			m_strPickModelTag = iter;
	}
	ImGui::EndListBox();

	if (ImGui::Button("Create_AnimModel"))
	{
		Create_Animation_Model(m_strPickModelTag);
	}
	if (m_pCurrentAnimModel != nullptr) {
		ImVec2 vSize2 = ImVec2(450, 100);
		ImGui::BeginListBox("Animation", vSize2);
		_uint iSize = m_pCurrentAnimModel->Get_Animations().size();
		for (_uint i = 0; i < iSize; i++)
		{
			string str = to_string(i);
			string str2;
			//size_t pos = m_vecObjectDemo[i]->Get_ModelTag().rfind(L"_");
			wstring wstr = m_pCurrentAnimModel->Get_Animations()[i]->Get_Name();
			str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
			if (ImGui::Selectable((str + "." + str2).c_str(), i == m_iCurrentAnimation)) {
				m_iCurrentAnimation = i;
			}
		}
		ImGui::EndListBox();

		if (ImGui::Button("Change_Anim"))
		{
			m_pCurrentAnimModel->Set_AnimationIndex(m_iCurrentAnimation);
		}

		if (ImGui::Button("Play"))
		{
			m_pCurrentAnimModel->Set_Anim_Loop(true);
			m_pCurrentAnimModel->Set_Anim_Play(true);
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			m_pCurrentAnimModel->Set_Anim_Play(false);
		}

		Arrow_Button("Speed", 0.1f, 
			&m_pCurrentAnimModel->Get_Animations()[m_iCurrentAnimation]->Get_ExtraSpeed()[0]);
		Arrow_Button("InterverSpeed", 0.1f,
			&m_pCurrentAnimModel->Get_Animations()[m_iCurrentAnimation]->Get_ExtraSpeed()[1]);

		ImGui::BeginListBox("Reserve", vSize);
		iSize = m_vecAnimationIndex.size();
		for (_uint i = 0; i < iSize; i++)
		{
			_uint iIndex = 0;

			string str = to_string(m_vecAnimationIndex[i]);
			string str2;
			wstring wstr = m_pCurrentAnimModel->Get_Animations()[m_vecAnimationIndex[i]]->Get_Name();
			str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
			if (ImGui::Selectable((str + "." + str2).c_str(), i == iIndex)) {
				
			}
		}
		ImGui::EndListBox();

		if (ImGui::Button("Add_Anim"))
		{
			if(!m_bReserve)
				m_vecAnimationIndex.push_back(m_iCurrentAnimation);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			m_vecAnimationIndex.clear();
		}
		ImGui::SameLine();
		if (ImGui::Button("Start"))
		{
			if (!m_bReserve) {
				m_pCurrentAnimModel->Set_Anim_Play(true);
				m_bReserve = true;
			}
		}
		if (m_bReserve)
			m_bReserve = Reserve_Animation();
	}
}

void CAnimation_Window::SubObject()
{

	ImGui::Text("SubObject_List");
	ImVec2 vSize = ImVec2(250, 100);
	ImGui::BeginListBox("Sub_Proto", vSize);
	for (auto& iter : m_vecSubPrototypeTag) {

		size_t pos = iter.rfind(L"_");
		wstring wstr = iter.substr(pos + 1);
		string str;
		str.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable(str.c_str(), iter == m_strPickSubModelTag))
			m_strPickSubModelTag = iter;
	}
	ImGui::EndListBox();

	ImGui::BeginListBox("SubObject", vSize);
	_uint iSize = m_vecSubObjectDemo.size();
	for (_uint i = 0; i < iSize; i++)
	{
		string str2;
		size_t pos = m_vecSubObjectDemo[i]->Get_ModelTag().rfind(L"_");
		wstring wstr = m_vecSubObjectDemo[i]->Get_ModelTag().substr(pos + 1);
		str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable((str2).c_str(), i == m_iCurrentDemoIndex)) {
			m_iCurrentDemoIndex = i;
		}
	}
	ImGui::EndListBox();

	if (ImGui::Button("Create_SubObj"))
	{
		Create_Sub_Model(m_strPickSubModelTag);
	}

	if (m_pCurrentAnimModel != nullptr) {
		ImGui::BeginListBox("Bone", vSize);
		iSize = m_pCurrentAnimModel->Get_Bones().size();
		for (_uint i = 0; i < iSize; i++)
		{
			string str = to_string(i);
			string str2;
			wstring wstr = m_pCurrentAnimModel->Get_Bone(i)->Get_BoneName_wstr();
			str2.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
			if (ImGui::Selectable((str + "." + str2).c_str(), i == m_iCurrentAnimModelBoneIndex)) {
				m_iCurrentAnimModelBoneIndex = i;
			}
		}
		ImGui::EndListBox();
		
		if (!m_vecSubObjectDemo.empty())
		{
			if (ImGui::Button("Apply"))
			{
				m_vecSubObjectDemo[m_iCurrentDemoIndex]->
					Set_SocketBone(m_pCurrentAnimModel->Get_Bone(m_iCurrentAnimModelBoneIndex));
				//m_vecSubObjectDemo[m_iCurrentDemoIndex]->Set_SocketBoneIndex(m_iCurrentAnimModelBoneIndex);
			}
		}
		
		ImGui::Checkbox("NonBlend", &m_bNonBlend);
		if (m_bNonBlend)
			m_pCurrentAnimModel->Set_NonBlendIndx(m_iCurrentAnimModelBoneIndex);			
		else
			m_pCurrentAnimModel->Set_NonBlendIndx(-1);
	}


	if (!m_vecSubObjectDemo.empty() && m_vecSubObjectDemo[m_iCurrentDemoIndex] != nullptr) {

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
		__super::ImGuizmo(ImGuizmo::MODE::WORLD, m_vecSubObjectDemo[m_iCurrentDemoIndex]);
	}
}

void CAnimation_Window::Create_Animation_Model(const wstring& strModelTag)
{
	if (m_pCurrentAnimModel == nullptr) {
		CAnimMesh_Demo::ANIMDEMOVALUE AnimDemo_Desc = {};

		AnimDemo_Desc.strModelTag = strModelTag;
		AnimDemo_Desc.vPos = _float4(0.f, 0.f, 0.f, 1.f);

		if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), TEXT("Tool"), G0_ANIMMESH_DEMO_TAG,
			&AnimDemo_Desc, reinterpret_cast<CGameObject**>(&m_pCurrentAnimModel))))
			return;

		//Safe_AddRef(m_pCurrentAnimModel);
	}
	else {
		m_pCurrentAnimModel->Set_Dead();
		m_pCurrentAnimModel = nullptr;

		CAnimMesh_Demo::ANIMDEMOVALUE AnimDemo_Desc = {};

		AnimDemo_Desc.strModelTag = strModelTag;
		AnimDemo_Desc.vPos = _float4(0.f, 0.f, 0.f, 1.f);

		if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), TEXT("Tool"), G0_ANIMMESH_DEMO_TAG,
			&AnimDemo_Desc, reinterpret_cast<CGameObject**>(&m_pCurrentAnimModel))))
			return;
	}
}

void CAnimation_Window::Create_Sub_Model(const wstring& strSubModelTag)
{
	if (m_pCurrentAnimModel == nullptr)
		return;

	CGameObject* pSubObject = nullptr;
	CSubObject_Demo::SUBOBJECTDEMO_DESC SubDemo_Desc = {};

	SubDemo_Desc.pParentsTransform = m_pCurrentAnimModel->Get_Component<CTransform>();
	SubDemo_Desc.strModelTag = strSubModelTag;
	SubDemo_Desc.vPos = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), TEXT("Tool"), G0_SUBMESH_DEMO_TAG,
		&SubDemo_Desc, reinterpret_cast<CGameObject**>(&pSubObject))))
		return;

	m_vecSubObjectDemo.push_back(dynamic_cast<CSubObject_Demo*>(pSubObject));
}

_bool CAnimation_Window::Reserve_Animation()
{
	if (m_pCurrentAnimModel == nullptr || m_vecAnimationIndex.empty())
	{
		m_iCurrent_Anim_Count = 0;
		m_pCurrentAnimModel->Set_Anim_Play(false);
		return false;
	}

	m_pCurrentAnimModel->Set_Anim_Loop(false);

	if (m_iCurrent_Anim_Count == 0)
	{
		m_pCurrentAnimModel->Set_AnimationIndex(m_vecAnimationIndex[m_iCurrent_Anim_Count]);
		++m_iCurrent_Anim_Count;
	}
	else {
		if (m_pCurrentAnimModel->Is_Finished_Animation())
		{
			if (m_iCurrent_Anim_Count >= m_vecAnimationIndex.size()) {
				m_iCurrent_Anim_Count = 0;
				m_pCurrentAnimModel->Set_Anim_Play(false);
				return false;
			}

			m_pCurrentAnimModel->Set_AnimationIndex(m_vecAnimationIndex[m_iCurrent_Anim_Count]);
			++m_iCurrent_Anim_Count;
		}
	}

	return true;
}

CAnimation_Window* CAnimation_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg)
{
	CAnimation_Window* pInstance = new CAnimation_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CAnimation_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimation_Window::Free()
{
	__super::Free();

	//Safe_Release(m_pCurrentAnimModel);
}

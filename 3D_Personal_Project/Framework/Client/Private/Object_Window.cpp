#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "..\Public\Object_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"

#include "ObjectMesh_Demo.h"

CObject_Window::CObject_Window()
{
}

HRESULT CObject_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	m_vecModelTag.push_back(TEXT("Prototype_Component_Model_PineTree"));

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
			ObjectMesh();

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

void CObject_Window::Picked(_float4 vPickPoint)
{
	if (m_pTerrain == nullptr)
		return;

	Create_Model(m_strPickModelTag, vPickPoint);
}

void CObject_Window::ObjectMesh()
{
	ImGui::Text("Object_List");
	ImVec2 vSize = ImVec2(250, 100);
	ImGui::BeginListBox("Model", vSize);
	for (auto& iter : m_vecModelTag) {
		string str;
		str.assign(iter.begin(), iter.end());// ���� ������ ��
		if (ImGui::Selectable(str.c_str(), iter == m_strPickModelTag))
			m_strPickModelTag = iter;
	}
	ImGui::EndListBox();

	ImGui::BeginListBox("Demo", vSize);
	for (_uint i = 0; i < m_vecDemo.size(); i++)
	{
		string str = to_string(i);
		string str2;
		str2.assign(m_strPickModelTag.begin(), m_strPickModelTag.end());// ���� ������ ��
		if (ImGui::Selectable((str + "." + str2).c_str(), i == m_iCurrentDemoIndex)) {
			m_iCurrentDemoIndex = i;
			m_strCurrentDemoTag = (str + "." + str2);
		}
	}
	ImGui::EndListBox();

	if (ImGui::Button("Delete Object")) {
		if (m_vecDemo[m_iCurrentDemoIndex] != nullptr)
		{
			//Safe_Release(m_vecDemo[m_iCurrentDemoIndex]);
			m_vecDemo.erase(m_vecDemo.begin() + m_iCurrentDemoIndex);
		}
	}

	ImGui::Separator();
	string strMessage = "Selected : " + m_strCurrentDemoTag;
	ImGui::Text(strMessage.c_str());

	if (!m_vecDemo.empty()&& m_vecDemo[m_iCurrentDemoIndex] != nullptr) {

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

void CObject_Window::ObjectMesh_Update()
{
}

void CObject_Window::Create_Model(const wstring& strModelTag, _float4 vPickPos)
{
	CGameObject* pObject_Demo = nullptr;
	CObjectMesh_Demo::OBDEMOVALUE ObjectDemoValue;

	ObjectDemoValue.strModelTag = strModelTag;
	ObjectDemoValue.vPos = vPickPos;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, TEXT("Tool"), TEXT("Prototype_GameObject_ObjectMesh_Demo"),
		&ObjectDemoValue,reinterpret_cast<CGameObject**>(&pObject_Demo))))
		return;

	m_vecDemo.push_back(dynamic_cast<CObjectMesh_Demo*>(pObject_Demo));
}

CObject_Window* CObject_Window::Create(void* pArg)
{
	CObject_Window* pInstance = new CObject_Window();

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

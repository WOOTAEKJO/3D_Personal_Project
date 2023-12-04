#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Public/ImGui_Window.h"
#include "GameInstance.h"

#include "ImGuiMgr.h"

#include "Demo.h"

typedef struct tagImGui_Window_Desc
{
	string	strName;	// 창 이름
	ImGuiWindowFlags window_flags;	// 창 옵션
	ImVec2	vWinSize;	// 창 사이즈
	ImVec4 vBackGroundColor = ImVec4(1.f, 1.f, 1.f, 1.f);  // 백 창 색

}IMGUIWINDESC;

CImGui_Window::CImGui_Window()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImGui_Window::Initialize(void* pArg)
{
	m_pDesc = pArg;

	m_pTerrain = CImGuiMgr::GetInstance()->Get_Terrain();
	Safe_AddRef(m_pTerrain);

	return S_OK;
}

void CImGui_Window::ImGuizmo(ImGuizmo::MODE eMode, CDemo* pDemo)
{
	ImGuizmo::SetOrthographic(false);
	//ImGuizmo::SetDrawlist(); 
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	static ImGuizmo::MODE mCurrentGizmoMode(eMode);

	_float4x4 matView = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW);
	_float arrView[] = { matView._11,matView._12,matView._13,matView._14,
					matView._21,matView._22,matView._23,matView._24,
					matView._31,matView._32,matView._33,matView._34,
					matView._41,matView._42,matView._43,matView._44 };

	_float4x4 matProj = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ);
	_float arrProj[] = { matProj._11,matProj._12,matProj._13,matProj._14,
					matProj._21,matProj._22,matProj._23,matProj._24,
					matProj._31,matProj._32,matProj._33,matProj._34,
					matProj._41,matProj._42,matProj._43,matProj._44 };

	_float4x4 matWorld = pDemo->Get_WorldMatrix();
	_float arrWorld[] = { matWorld._11,matWorld._12,matWorld._13,matWorld._14,
					 matWorld._21,matWorld._22,matWorld._23,matWorld._24,
					 matWorld._31,matWorld._32,matWorld._33,matWorld._34,
					 matWorld._41,matWorld._42,matWorld._43,matWorld._44 };

	_float fPos[3], fRot[3], fScale[3];
	
	ImGuizmo::DecomposeMatrixToComponents(arrWorld, fPos, fRot, fScale);

	Arrow_Button("Interval", 0.1f, &m_fInterval);

	switch (m_eOperationType)
	{
	case ImGuizmo::TRANSLATE:
		Arrow_Button("Pos_X",m_fInterval ,&fPos[0]);
		Arrow_Button("Pos_Y", m_fInterval, &fPos[1]);
		Arrow_Button("Pos_Z", m_fInterval, &fPos[2]);
		break;
	case ImGuizmo::ROTATE:
		Arrow_Button("Pos_X", m_fInterval, &fRot[0]);
		Arrow_Button("Pos_Y", m_fInterval, &fRot[1]);
		Arrow_Button("Pos_Z", m_fInterval, &fRot[2]);
		break;
	case ImGuizmo::SCALE:
		Arrow_Button("Pos_X", m_fInterval, &fScale[0]);
		Arrow_Button("Pos_Y", m_fInterval, &fScale[1]);
		Arrow_Button("Pos_Z", m_fInterval, &fScale[2]);
		break;
	default:
		break;
	}
	
	ImGuizmo::RecomposeMatrixFromComponents(fPos, fRot, fScale, arrWorld);

	Arrow_Button("ImGuizmo_Snap", 0.1f, &m_fSnap);

	ImGuizmo::Manipulate(arrView, arrProj, m_eOperationType, mCurrentGizmoMode, arrWorld, NULL, &m_fSnap);

	_float4x4 matW = { arrWorld[0],arrWorld[1],arrWorld[2],arrWorld[3],
				arrWorld[4],arrWorld[5],arrWorld[6],arrWorld[7],
				arrWorld[8],arrWorld[9],arrWorld[10],arrWorld[11],
				arrWorld[12],arrWorld[13],arrWorld[14],arrWorld[15] };

	pDemo->Set_WorldMatrix(matW);
	
	ImGuizmo::SetDrawlist();
}

void CImGui_Window::Set_File_Flag(TYPE eType)
{
	//OPENFILENAME ofn;

	ZeroMemory(&m_ofn, sizeof(OPENFILENAME));
	m_ofn.lStructSize = sizeof(OPENFILENAME);
	m_ofn.hwndOwner = NULL;
	m_ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
	m_ofn.lpstrFile = m_szFile;
	m_ofn.nMaxFile = MAX_PATH;
	if (eType == TYPE::TYPE_SAVE) {
		//m_ofn.lpstrDefExt = L"json";
		m_ofn.Flags = OFN_OVERWRITEPROMPT;
	}
	else {
		m_ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	}	

	SetCurrentDirectory(m_ofn.lpstrFile);
}

string CImGui_Window::wstring_To_Json(const wstring& strTag)
{
	wstring_convert<codecvt_utf8<_tchar>> converter;
	string utf8Str = converter.to_bytes(strTag);

	return utf8Str;
}

wstring CImGui_Window::Json_To_wstring(const string& utf8Str)
{
	int length = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);

	if (length > 0)
	{
		wstring wstr(length - 1, L'\0');

		MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], length - 1);
		return wstr;
	}
	return L"";
}

void CImGui_Window::Begin()
{
	IMGUIWINDESC* pDesc = (IMGUIWINDESC*)m_pDesc;

	ImGui::PushStyleColor(ImGuiCol_PopupBg, pDesc->vBackGroundColor);

	ImGui::SetNextWindowSize(pDesc->vWinSize, 0);

	ImGui::SetNextWindowBgAlpha(0.5f);

	ImGui::Begin(pDesc->strName.c_str(), 0, pDesc->window_flags);

}

void CImGui_Window::End()
{
	ImGui::PopStyleColor();

	ImGui::End();
}

void CImGui_Window::Arrow_Button(const string& strTag, _float fInterval, float* fValue)
{
	ImGui::InputFloat(strTag.c_str(), fValue);
	ImGui::SameLine();
	string str = "##Up" + strTag;
	if (ImGui::ArrowButton(str.c_str(), ImGuiDir_Up))
		*fValue += fInterval;
	ImGui::SameLine();
	str = "##Down" + strTag;
	if (ImGui::ArrowButton(str.c_str(), ImGuiDir_Down))
		*fValue -= fInterval;
}

void CImGui_Window::Free()
{
	__super::Free();

	free(m_pDesc);
	//Safe_Delete(m_pDesc);
	Safe_Release(m_pTerrain);
	Safe_Release(m_pGameInstance);
}

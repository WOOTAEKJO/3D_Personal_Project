#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <iostream>

using namespace std;

typedef struct tagImGui_Window_Desc
{
	string	strName;	// â �̸�
	ImGuiWindowFlags window_flags;	// â �ɼ�
	ImVec2	vWinSize;	// â ������
	ImVec4 vBackGroundColor = ImVec4(1.f, 1.f, 1.f, 1.f);  // �� â ��

}IMGUIWINDESC;
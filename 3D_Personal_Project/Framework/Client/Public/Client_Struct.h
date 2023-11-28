#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <iostream>

using namespace std;

typedef struct tagImGui_Window_Desc
{
	string	strName;	// 창 이름
	ImGuiWindowFlags window_flags;	// 창 옵션
	ImVec2	vWinSize;	// 창 사이즈
	ImVec4 vBackGroundColor = ImVec4(1.f, 1.f, 1.f, 1.f);  // 백 창 색

}IMGUIWINDESC;
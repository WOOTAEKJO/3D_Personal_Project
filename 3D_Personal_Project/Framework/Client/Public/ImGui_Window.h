#pragma once
//#include "imgui.h"
//#include "imgui_impl_win32.h"
//#include "imgui_impl_dx11.h"
#include "Client_Defines.h"
#include "Base.h"
#include "ImGuiMgr.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)

class CImGui_Window abstract : public CBase
{
public:
	//typedef struct tagImGui_Window_Desc
	//{
	//	string	strName;	// â �̸�
	//	ImGuiWindowFlags window_flags;	// â �ɼ�
	//	ImVec2	vWinSize;	// â ������
	//	ImVec4 vBackGroundColor = ImVec4(1.f, 1.f, 1.f, 1.f);  // �� â ��

	//}IMGUIWINDESC;
protected:
	CImGui_Window();
	virtual	~CImGui_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg);
	virtual	void	Tick() = 0;
	virtual	HRESULT	Render() = 0;

protected:
	CGameInstance*	m_pGameInstance = { nullptr };

protected:
	//string	m_strName;
	//ImGuiWindowFlags m_window_flags = {};
	////ImVec2 m_vWinSize;
	//ImVec2 m_vWinSize;
	//ImVec4 m_vBackGroundColor;
	void*	m_pDesc = { nullptr };

protected:
	void	Begin();
	void	End();

public:
	virtual	void	Free() override;
};

END
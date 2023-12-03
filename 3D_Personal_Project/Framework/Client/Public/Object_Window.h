#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CObjectMesh_Demo;

class CObject_Window final : public CImGui_Window
{
public:
	typedef struct tagObject_Window_Desc
	{
	}OBJECTWINDOWDESC;
private:
	CObject_Window();
	virtual	~CObject_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg) override;
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;

private:
	vector<wstring>					m_vecModelTag;
	wstring							m_strPickModelTag;

	vector<CObjectMesh_Demo*>		m_vecDemo;
	_uint							m_iCurrentDemoIndex = { 0 };
	string							m_strCurrentDemoTag;
	
private: /* For.RadioButton_Transform*/
	_int	m_iTransformRadioButton = {0};
	

private:
	void	ObjectMesh();
	void	Create_Model(const wstring& strModelTag, _float4 vPickPos);
	void	NotGuizmo();

public:
	static	CObject_Window* Create(void* pArg);
	virtual	void	Free() override;
};

END
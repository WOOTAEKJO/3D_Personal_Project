#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CObjectMesh_Demo;
class CAnimMesh_Demo;

class CObject_Window final : public CImGui_Window
{
public:
	enum TYPE {TYPE_NONANIM, TYPE_ANIM,TYPE_END};

	typedef struct tagObject_Window_Desc
	{
	}OBJECTWINDOWDESC;
private:
	CObject_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CObject_Window() = default;

public:
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	void	Tick() override;
	virtual	HRESULT	Render() override;
	virtual	void	Set_Variable(void* pArg) override;
	virtual	void	Terrain_Picked(_float4 vPickPoint) override;
	virtual	void	Demo_Picked() override;
	virtual	string	Get_Path() override;
	virtual	HRESULT	Save_Data(const _char* strFilePath) override;
	virtual	HRESULT	Load_Data(const _char* strFilePath) override;

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

public:
	vector<CObjectMesh_Demo*>* Get_ObjectDemo() { return &m_vecDemo; }

private:
	vector<wstring>					m_vecModelTag;
	wstring							m_strPickModelTag;

	vector<CObjectMesh_Demo*>		m_vecDemo;
	_uint							m_iCurrentDemoIndex = { 0 };
	string							m_strCurrentDemoTag;

private:
	vector<wstring>					m_vecAnimModelTag;
	wstring							m_strPickAnimModelTag;

	vector<CAnimMesh_Demo*>			m_vecAnimDemo;
	_uint							m_iCurrentAnimDemoIndex = { 0 };
	string							m_strCurrentAnimDemoTag;
	
private: /* For.RadioButton_Transform*/
	_int	m_iTransformRadioButton = {0};
	
	TYPE	m_eCurrentType = { TYPE_END };

	_uint	m_iCurrentLayerName = { LAYER::LAYER_END };

private:
	void	ObjectMesh();
	void	AnimObjectMesh();
	void	Create_Model(const wstring& strLayerTag, const wstring& strModelTag, _float4 vPickPos);
	void	Create_AnimModel(const wstring& strLayerTag, const wstring& strModelTag, _float4 vPickPos);
	void	NotGuizmo();

public:
	static	CObject_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg);
	virtual	void	Free() override;
};

END
#pragma once
#include "ImGui_Window.h"

BEGIN(Client)

class CObjectMesh_Demo;
class CAnimMesh_Demo;

class CObject_Window final : public CImGui_Window
{
public:
	enum TYPE {TYPE_NONANIM, TYPE_ANIM,TYPE_INSTANCING, TYPE_END};

	typedef struct tagObject_Window_Desc
	{
	}OBJECTWINDOWDESC;

	typedef struct tagObject_Demo_Desc
	{
		vector<wstring>					vecModelTag;
		wstring							strPickModelTag;

		vector<CObjectMesh_Demo*>		vecDemo;
		vector<CAnimMesh_Demo*>			vecAnimDemo;
		_uint							iCurrentIndex = { 0 };
		string							strCurrentTag;

	}OBJECTDEMO_DESC;

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

	_int							m_iNaviIndxRadiButton = { -1 };

private:
	OBJECTDEMO_DESC					m_eInstancingModel;
	OBJECTDEMO_DESC					m_eTMP;

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
	_float	m_fTimeAcc = { 0.f };

private:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;
	_bool			m_bRandomRotation[3] = { false,false,false };

private:
	void	ObjectMesh();
	void	AnimObjectMesh();
	void	InstancingMesh();
	void	Create_Model(const wstring& strLayerTag, const wstring& strModelTag, _float4 vPickPos);
	void	Create_AnimModel(const wstring& strLayerTag, const wstring& strModelTag, _float4 vPickPos);
	void	Create_TMP(const wstring& strLayerTag, const wstring& strModelTag, _float4 vPickPos);
	void	Create_Instancing();
	void	NotGuizmo();
	void	TransformGuizmo();
	_float3	Random_Roation();

public:
	static	CObject_Window* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg);
	virtual	void	Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "Production.h"

BEGIN(Engine)

END

BEGIN(Client)

class CUI;
class CTargetCamera;

class CClient_Production abstract : public CProduction
{
public:
	typedef struct tagActorInfo
	{
		_float3 vOffset;
		_float fSensitivity;
		wstring strActorTag;
		_int	iAnimIndex = { -1 };
		_float	fLookHeight = { 1.f };


	}PRODUCTION_ACTOR_DESC;

protected:
	CClient_Production();
	virtual	~CClient_Production() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual	void	Enter() override;
	virtual	void	Tick() override;
	virtual	void	Render() override;
	virtual	void	Exite() override;

protected:
	CTargetCamera*	m_pCamera = { nullptr };

protected:
	vector<PRODUCTION_ACTOR_DESC> m_vecActorOrder;
	_uint						m_iCurrentOrderIndx = { 0 };

protected:
	//vector<CUI*>		m_vecUI;
	map<wstring, list<CUI*>> m_mapUI;
	wstring					m_strActivateGroupTag;

protected:
	vector<wstring>		m_vecUIOrder;
	_uint				m_iCurrentUIOrderIndx = {0};

	vector<PRODUCTION_DIALOG_DESC>		m_vecFont;
	_uint				m_iCurrentFontIndx = { 0 };

protected:
	_bool	SetUp_Camera();
	void	Camera_Target_Change(_bool bSpring = false);
	void	Camera_Reset();
	_float3	Compute_Offset(const wstring& strActorTag,_float fX, _float fY, _float fZ);
	_float3	Compute_Offset_Transform(CTransform* pActorTramsform, _float fLook, _float fRight);

protected:
	virtual	HRESULT Ready_UI();
	virtual	void	Ready_Font();

protected:
	CUI* Add_UI(_float2 vCenterPos, _float2 vScale, const wstring& strTextureTag,const wstring& strProtoTag);
	HRESULT	Add_GROUP(const wstring& strGroupTag,CUI* pUI);
	list<CUI*>* Find_UIGROUP(const wstring& strGroupTag);

	void	SetUp_UI_Group(const wstring& strGroupTag);
	void	Clear_UI_Group();

	void	RenderUI();
	void	RenderFont();

protected:


public:
	virtual	void	Free();
};

END
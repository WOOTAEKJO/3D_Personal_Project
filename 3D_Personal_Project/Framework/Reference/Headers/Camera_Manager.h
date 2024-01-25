#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameInstance;
class CCamera;

class CCamera_Manager final : public CBase
{
private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Initialize();

	HRESULT	Add_Camera(const wstring& strCameraTag, CCamera* pCamera);
	void	SetUp_Offset(_float3 vOffset);
	void	SetUp_Camera(const wstring& strCameraTag);

private:
	map<wstring, CCamera*> m_mapCamera;

private:
	wstring			m_strCurrentCameraTag;

private:
	CCamera* Find_Camera(const wstring& strCameraTag);

public:
	static CCamera_Manager* Create();
	virtual void	Free();
};

END
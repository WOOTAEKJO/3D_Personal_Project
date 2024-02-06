#pragma once
#include "Base.h"

BEGIN(Engine)

class CLight;
class CShadowLight;
class CShader;
class CVIBuffer_Rect;

class CLight_Manager final : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT	Add_Light(const LIGHT_DESC& eLightDesc, _Out_ CLight** ppLight);
	void Delete_Light(CLight* ppLight);
	HRESULT	Render(CShader* pShader, CVIBuffer_Rect* pBuffer);

public:
	HRESULT	Add_ShadowLight(const SHADOW_LIGHT_DESC& eLightDesc);
	CShadowLight* Get_ShadowLight();

private:
	list<CLight*>	m_listLight;
	CShadowLight*	m_pShadowLight = { nullptr };

public:
	static CLight_Manager* Create();
	virtual	void	Free();
};

END
#pragma once
#include "Base.h"

BEGIN(Engine)

class CLight;
class CShader;
class CVIBuffer_Rect;

class CLight_Manager final : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT	Add_Light(const LIGHT_DESC& eLightDesc);
	HRESULT	Render(CShader* pShader, CVIBuffer_Rect* pBuffer);

private:
	list<CLight*>	m_listLight;

public:
	static CLight_Manager* Create();
	virtual	void	Free();
};

END
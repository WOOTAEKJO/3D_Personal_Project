#pragma once
#include "Base.h"

BEGIN(Engine)

class CShader;
class CVIBuffer_Rect;

class CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	HRESULT  Initialize(const LIGHT_DESC& eLightDesc);
	HRESULT  Render(CShader* pShader, CVIBuffer_Rect* pBuffer);

private:
	LIGHT_DESC m_eLight_Desc;

public:
	static CLight* Create(const LIGHT_DESC& eLightDesc);
	virtual	 void	Free();
};

END
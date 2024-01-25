#pragma once
#include "Base.h"

BEGIN(Engine)

class CShader;
class CVIBuffer_Rect;
class CGameObject;
class CGameInstance;

class ENGINE_DLL CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	HRESULT  Initialize(const LIGHT_DESC& eLightDesc);
	HRESULT  Render(CShader* pShader, CVIBuffer_Rect* pBuffer);

public:
	void	Update_Pos(_fvector vPos);

public:
	LIGHT_DESC* Open_Light_Desc() { return &m_eLight_Desc; }
	void		Set_Active(_bool bCheck) { m_bActive = bCheck; }

private:
	CGameInstance* m_pGameInstance = { nullptr };

private:
	LIGHT_DESC m_eLight_Desc;
	_bool		m_bActive = { true };

public:
	static CLight* Create(const LIGHT_DESC& eLightDesc);
	virtual	 void	Free();
};

END
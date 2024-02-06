#pragma once
#include "Base.h"

BEGIN(Engine)

class CShader;
class CVIBuffer_Rect;
class CGameObject;
class CGameInstance;

class ENGINE_DLL CShadowLight final : public CBase
{
public:
	enum STATE {VIEW,PROJ,STATE_END};

private:
	CShadowLight();
	virtual ~CShadowLight() = default;

public:
	HRESULT  Initialize(const SHADOW_LIGHT_DESC & eLightDesc);

public:
	SHADOW_LIGHT_DESC* Open_Light_Desc() { return &m_eLight_Desc; }
	void	Set_Light_Desc(SHADOW_LIGHT_DESC Desc) { m_eLight_Desc = Desc; }
	void	SetUp_Matrix();
	_float4x4	Get_Matrix(STATE eType) { return m_matTransform[eType]; }
	void		Set_Matrix(STATE eType, _float4x4 mat) { m_matTransform[eType] = mat; }

private:
	CGameInstance* m_pGameInstance = { nullptr };

private:
	SHADOW_LIGHT_DESC m_eLight_Desc;

private:
	_float4x4		m_matTransform[STATE::STATE_END];

public:
	static CShadowLight* Create(const SHADOW_LIGHT_DESC& eLightDesc);
	virtual	 void	Free();
};

END
#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CParticleData : public CBase
{
private:
	CParticleData();
	virtual ~CParticleData() = default;

public:
	HRESULT	Initialize();
	HRESULT	Save_Data(const char* strPath);
	HRESULT	Load_Data(const char* strPath);

public:
	void	Set_Data(INSTANCING_DESC Instance_Desc) { m_Instancing_Desc = Instance_Desc; }
	INSTANCING_DESC Get_Data() { return m_Instancing_Desc; }

private:
	INSTANCING_DESC	m_Instancing_Desc = {};

public:
	static CParticleData* Create();
	virtual	void	Free() override;

};

END
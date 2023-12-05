#pragma once
#include "Base.h"

BEGIN(Engine)

class CVIBuffer_DTerrain;

class CSaveLoad_Manager final : public CBase
{
private:
	CSaveLoad_Manager();
	virtual	~CSaveLoad_Manager() = default;

public:
	HRESULT	Initialize();
	HRESULT	Save_Data(const _char* strFileName);
	HRESULT	Load_Data_Terrain(CVIBuffer_DTerrain* pDTerrain,const _char* strFileName);

private:

public:
	static	CSaveLoad_Manager* Create();
	virtual	void	Free() override;
};

END
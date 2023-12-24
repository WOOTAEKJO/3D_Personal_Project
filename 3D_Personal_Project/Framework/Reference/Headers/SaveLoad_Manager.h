#pragma once
#include "Base.h"
#include "MeshData.h"


BEGIN(Engine)

class CSaveLoad_Manager final : public CBase
{
private:
	CSaveLoad_Manager();
	virtual	~CSaveLoad_Manager() = default;

public:
	HRESULT	Initialize();
	HRESULT	Save_Data_Mesh(const _char* strFileName,CMeshData::MESHDATADESC MeshDataDesc);
	HRESULT	Load_Data_Mesh(CVIBuffer* pBuffer,const _char* strFileName);

public:
	HRESULT	Save_Data_Json(const _char* strFilePath, CGameObject* pObject);
	HRESULT	Load_Data_Json(const wstring& strTag, CGameObject* pObject);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static	CSaveLoad_Manager* Create();
	virtual	void	Free() override;
};

END
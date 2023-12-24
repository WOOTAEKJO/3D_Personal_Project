#pragma once
#include "Base.h"
#include "MeshData.h"


BEGIN(Engine)

class CSaveLoad_Manager final : public CBase
{
public:
	enum PAHT_TYPE {TYPE_DATA,TYPE_SHADER,TYPE_RESOURCE,TYPE_END};

private:
	CSaveLoad_Manager();
	virtual	~CSaveLoad_Manager() = default;

public:
	HRESULT	Initialize();
	HRESULT	Save_Data_Mesh(const _char* strFileName,CMeshData::MESHDATADESC MeshDataDesc);
	HRESULT	Load_Data_Mesh(CVIBuffer* pBuffer,const _char* strFileName);

public:
	wstring	PathFinder(wstring strTag, PAHT_TYPE eType);
	void	Setting_FilePath(const wstring& strDataPath, const wstring& strShaderPath, const wstring& strResourcePath);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	wstring				m_strDataFilePath;
	wstring				m_strShaderFilePath;
	wstring				m_strResourceFilePath;

public:
	static	CSaveLoad_Manager* Create();
	virtual	void	Free() override;
};

END
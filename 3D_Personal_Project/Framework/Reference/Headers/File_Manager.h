#pragma once
#include "Base.h"

BEGIN(Engine)

class CFile_Manager final : public CBase
{
public:
	enum PAHT_TYPE { TYPE_DATA, TYPE_SHADER, TYPE_RESOURCE, TYPE_END };

	typedef struct tagFile_Desc
	{
		wstring FileName;
		wstring FilePath;
	}FILE_DESC;

private:
	CFile_Manager();
	virtual	~CFile_Manager() = default;

public:
	HRESULT	Initialize(const wstring& strFilePath);

public:
	void	Setting_FilePath(const wstring& strDataPath, const wstring& strShaderPath, const wstring& strResourcePath);

public:
	string	Load_Data_Path(wstring strTag);
	string	Load_ShaderFiles_Path(wstring strTag);
	string	Load_Models_Path(wstring strTag);
	string	Load_Texture_Path(wstring strTag);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	vector<FILE_DESC>		m_vecFilePath;

	wstring				m_strDataFilePath;
	wstring				m_strShaderFilePath;
	wstring				m_strResourceFilePath;

private:
	HRESULT		Setting_FilePath(const wstring& strFilePath);
	string		PathFinder(wstring strPath, wstring strTag);
	_bool		Is_Exc(const wstring& strFilePath);
	wstring		Find_File(const wstring& strFileName);
	_bool		Is_Number(const string& strFilePath);
	string		Remove_Number(const string& strFilePath);

public:
	static	CFile_Manager* Create(const wstring& strFilePath);
	virtual	void	Free() override;
};

END
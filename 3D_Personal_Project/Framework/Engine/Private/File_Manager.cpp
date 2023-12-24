#include "..\Public\File_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>

CFile_Manager::CFile_Manager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CFile_Manager::Initialize(const wstring& strFilePath)
{
	if (FAILED(Setting_FilePath(strFilePath)))
		return E_FAIL;

	return S_OK;
}

string CFile_Manager::PathFinder(wstring strPath, wstring strTag)
{
	string strTagName = CUtility_String::WString_To_string(CUtility_String::BackCut_Wstring(strTag, TEXT('_')));

	for (filesystem::directory_entry entry : filesystem::recursive_directory_iterator(strPath))
	{
		string strFIlePath = CUtility_String::WString_To_string( entry.path());
		string strTmp;

		_char	szFileName[MAX_PATH] = "";
		_char	szExc[MAX_PATH] = "";

		_splitpath_s(strFIlePath.c_str(), nullptr, 0, nullptr, 0,
			szFileName, MAX_PATH, szExc, MAX_PATH);

		strTmp = szExc;

		if (!strcmp(strTmp.c_str(), ""))
			continue;

		strTmp = szFileName;

		strTmp = CUtility_String::BackCut_string(strTmp, '_');

		if (!strcmp(strTagName.c_str(), strTmp.c_str()))
			return strFIlePath;
	}

	return string();
}

void CFile_Manager::Setting_FilePath(const wstring& strDataPath, const wstring& strShaderPath, const wstring& strResourcePath)
{
	m_strDataFilePath = strDataPath;
	m_strShaderFilePath = strShaderPath;
	m_strResourceFilePath = strResourcePath;
}

HRESULT CFile_Manager::Setting_FilePath(const wstring& strFilePath)
{
	vector<wstring> vecFile;

	for (auto& entry : filesystem::recursive_directory_iterator(strFilePath))
	{
		if (filesystem::is_directory(entry))
			vecFile.push_back(entry.path());
	}

	for (auto& iter : vecFile)
	{
		wstring strTmp;

		strTmp = CUtility_String::BackCut_Wstring(iter, TEXT('\\'));
		strTmp = CUtility_String::BackCut_Wstring(strTmp, TEXT('/'));

		if (wcscmp(strTmp.c_str(), TEXT("Data")) == 0 || wcscmp(strTmp.c_str(), TEXT("ShaderFiles")) == 0 ||
			wcscmp(strTmp.c_str(), TEXT("Models")) == 0 || wcscmp(strTmp.c_str(), TEXT("Textures")) == 0)
		{
			FILE_DESC FileDesc = {};

			FileDesc.FileName = strTmp;
			FileDesc.FilePath = iter;

			m_vecFilePath.push_back(FileDesc);
		}
	}

	if (m_vecFilePath.empty())
		return E_FAIL;

	return S_OK;
}

string CFile_Manager::Load_Data_Path(wstring strTag)
{
	wstring strFilePath = Find_File(TEXT("Data"));

	return PathFinder(strFilePath, strTag);
}

string CFile_Manager::Load_ShaderFiles_Path(wstring strTag)
{
	wstring strFilePath = Find_File(TEXT("ShaderFiles"));

	return PathFinder(strFilePath, strTag);
}

string CFile_Manager::Load_Models_Path(wstring strTag)
{
	wstring strFilePath = Find_File(TEXT("Models"));

	return PathFinder(strFilePath, strTag);
}

string CFile_Manager::Load_Texture_Path(wstring strTag)
{
	wstring strFilePath = Find_File(TEXT("Textures"));

	string strTagName = CUtility_String::WString_To_string(CUtility_String::BackCut_Wstring(strTag, TEXT('_')));

	for (filesystem::directory_entry entry : filesystem::recursive_directory_iterator(strFilePath))
	{
		string strFIlePath = CUtility_String::WString_To_string(entry.path());
		string strTmp;
		_bool	bNumCheck = false;

		_char	szDdrive[MAX_PATH] = "";
		_char	szDirectory[MAX_PATH] = "";
		_char	szFileName[MAX_PATH] = "";
		_char	szExc[MAX_PATH] = "";

		_splitpath_s(strFIlePath.c_str(), szDdrive, MAX_PATH, szDirectory, MAX_PATH,
			szFileName, MAX_PATH, szExc, MAX_PATH);

		strTmp = szExc;

		if (!strcmp(strTmp.c_str(), ""))
			continue;

		strTmp = szFileName;

		if (Is_Number(strTmp))
		{
			strTmp = Remove_Number(strTmp);
			bNumCheck = true;
		}

		if (!strcmp(strTagName.c_str(), strTmp.c_str())) {

			_char szFile[MAX_PATH] = "";

			strcpy_s(szFile, szDdrive);
			strcat_s(szFile, szDirectory);
			strcat_s(szFile, (strTmp+ (bNumCheck == true ? "%d" : "")).c_str());
			strcat_s(szFile, szExc);

			return szFile;
		}
	}
	return string();
}

_bool CFile_Manager::Is_Exc(const wstring& strFilePath)
{
	wstring strFileName;

	strFileName = CUtility_String::Cut_Wstring(strFilePath, TEXT('/'));
	wstring strTmp = CUtility_String::BackCut_Wstring(strFilePath, TEXT('.'));

	if (wcscmp(strTmp.c_str(), strFileName.c_str()) == 0)
		return false;

	return true;
}

wstring CFile_Manager::Find_File(const wstring& strFileName)
{
	if (m_vecFilePath.empty())
		return wstring();

	wstring strFilePath = TEXT("");

	for (auto& iter : m_vecFilePath)
	{
		if (wcscmp(strFileName.c_str(), iter.FileName.c_str()) == 0)
			strFilePath = iter.FilePath;
	}

	return strFilePath;
}

_bool CFile_Manager::Is_Number(const string& strFilePath)
{
	_uint iSize = strFilePath.size();

	if (strFilePath[iSize - 1] > 47 && strFilePath[iSize - 1] < 58)
		return true;

	return false;
}

string CFile_Manager::Remove_Number(const string& strFilePath)
{
	string strTmp = "";
	_uint iSize = strFilePath.size();

	if (strFilePath[iSize - 1] > 47 && strFilePath[iSize - 1] < 58)
	{
		for (_uint i = 0; i < iSize - 1; i++)
			strTmp.push_back(strFilePath[i]);
		
		strTmp = Remove_Number(strTmp);
	}
	else {
		strTmp = strFilePath;
	}

	return strTmp;
}

CFile_Manager* CFile_Manager::Create(const wstring& strFilePath)
{
	CFile_Manager* pInstance = new CFile_Manager();

	if (FAILED(pInstance->Initialize(strFilePath))) {
		MSG_BOX("Failed to Created : CFile_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFile_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}

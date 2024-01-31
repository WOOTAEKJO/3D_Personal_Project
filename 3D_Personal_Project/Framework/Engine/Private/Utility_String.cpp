#include "..\Public\Utility_String.h"
#include <sstream>

wstring CUtility_String::Split_Wstring(wstring strFull, _tchar cSeperator)
{
	wstring strTmp;
	wstringstream stream(strFull);
	getline(stream, strTmp, cSeperator);

	return strTmp;
}

string CUtility_String::Split_string(string strFull, _char cSeperator)
{
	string strTmp;
	stringstream stream(strFull);
	getline(stream, strTmp, cSeperator);

	return strTmp;
}

wstring CUtility_String::BackCut_Wstring(wstring strFull, _tchar cSeperator)
{
	_int iNum = strFull.rfind(cSeperator) + 1;
	return strFull.substr(iNum);
}

string CUtility_String::BackCut_string(string strFull, _char cSeperator)
{
	_int iNum = strFull.rfind(cSeperator) + 1;
	return strFull.substr(iNum);
}

wstring CUtility_String::Cut_Wstring(wstring strFull, _tchar cSeperator)
{
	_int iNum = strFull.find(cSeperator);
	return strFull.substr(iNum);
}

string CUtility_String::Cut_string(string strFull, _char cSeperator)
{
	_int iNum = strFull.find(cSeperator);
	return strFull.substr(iNum);
}

wstring CUtility_String::string_To_Wstring(string str)
{
	wstring strTmp;
	_uint iLength = str.length();
	return strTmp.assign(str.begin() + 0, str.begin() + iLength);
}

string CUtility_String::WString_To_string(wstring str)
{
	string strTmp;
	_uint iLength = str.length();
	return strTmp.assign(str.begin() + 0, str.begin() + iLength);
}

wstring CUtility_String::Standard_Cut_Wstring(_uint iCount, wstring strFull, _tchar cSeperator)
{
	wstring strTmp;
	_uint iPos = 0;

	
	strTmp = strFull;
	for (_uint i = 0; i < iCount; i++) {
		iPos = strTmp.find(cSeperator);
		strTmp = strTmp.substr(iPos + 1);
	}

	return strTmp;
}

string CUtility_String::Standard_Cut_string(_uint iCount, string strFull, _char cSeperator)
{
	string strTmp;
	_uint iPos = 0;

	for (_uint i = 0; i < iCount; i++) {
		iPos = strFull.find(cSeperator);
		strTmp = strFull.substr(iPos + 1);
	}

	return strTmp;
}

wstring CUtility_String::Remove_Numbers(wstring str)
{
	wstring strTmp;

	for (auto& iter : str)
	{
		if (iter < 48 || iter > 57)
			strTmp.push_back(iter);
	}

	return strTmp;
}

wstring CUtility_String::Get_MiddleName(wstring str)
{ 
	wstring strTmp;
	strTmp = Standard_Cut_Wstring(2, str, L'_');
	strTmp = Split_Wstring(strTmp, L'_');

	return strTmp;
}

wstring CUtility_String::Get_LastName(wstring str)
{
	wstring strTmp;
	strTmp = Standard_Cut_Wstring(3, str, L'_');
	strTmp = Split_Wstring(strTmp, L'_');

	return strTmp;
}

_bool CUtility_String::Compare_WString(wstring strSoul, wstring strDest)
{
	if (!wcscmp(strSoul.c_str(), strDest.c_str()))
	{
		return true;
	}

	return false;
}




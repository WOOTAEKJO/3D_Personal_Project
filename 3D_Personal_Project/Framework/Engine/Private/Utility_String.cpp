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



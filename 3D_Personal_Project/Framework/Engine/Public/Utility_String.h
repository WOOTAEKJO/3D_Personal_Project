#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CUtility_String
{
public:
	static wstring Split_Wstring(wstring strFull, _tchar cSeperator);
	static string Split_string(string strFull, _char cSeperator);

	static wstring BackCut_Wstring(wstring strFull, _tchar cSeperator);
	static string BackCut_string(string strFull, _char cSeperator);

	static wstring Cut_Wstring(wstring strFull, _tchar cSeperator);
	static string Cut_string(string strFull, _char cSeperator);

	static wstring string_To_Wstring(string str);
	static string WString_To_string(wstring str);

	static wstring Standard_Cut_Wstring(_uint iCount, wstring strFull, _tchar cSeperator);
	static string Standard_Cut_string(_uint iCount, string strFull, _char cSeperator);

	static wstring Remove_Numbers(wstring str);

	static wstring Get_MiddleName(wstring str);
	static wstring Get_LastName(wstring str);

	static _bool Compare_WString(wstring strSoul, wstring strDest);
};

END


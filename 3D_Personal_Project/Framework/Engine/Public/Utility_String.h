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
};

END


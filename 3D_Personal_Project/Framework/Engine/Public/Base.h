#pragma once

#include "Engine_Defines.h"
#include "Json/Json_Utility.h"

/* ���۷���ī��Ʈ�� �����ϱ����� ����� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* ���۷��� ī��Ʈ�� ������Ű��. */
	/* _ulong : ������Ű�� �� ������ ���� ����. */
	_ulong AddRef();

	/* ���۷��� ī��Ʈ�� ���ҽ�Ű�ų� �����ϰų�.. */
	/* _ulong : ���ҽ�Ű�� ������ ���� ���� .*/
	_ulong Release();

public:
	template<typename T>
	wstring TAG_NAME()
	{
		_tchar szFullPath[MAX_PATH] = TEXT("");
		const _char* szTmp = typeid(T).name();
		MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), szFullPath, MAX_PATH);
		return szFullPath;
	}

public:
	virtual void Write_Json(json& Out_Json) {};
	virtual void Load_FromJson(const json& In_Json) {};

private:
	_ulong			m_dwRefCnt = { 0 };

public:
	virtual void Free() {}
};

END
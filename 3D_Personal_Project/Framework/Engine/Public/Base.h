#pragma once

#include "Engine_Defines.h"
#include "Json/Json_Utility.h"

/* 레퍼런스카운트를 관리하기위한 기능을 제공한다. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* 레퍼런스 카운트를 증가시키낟. */
	/* _ulong : 증가시키고 난 이후의 값을 리턴. */
	_ulong AddRef();

	/* 레퍼런스 카운트를 감소시키거나 삭제하거나.. */
	/* _ulong : 감소시키기 이전의 값을 리턴 .*/
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
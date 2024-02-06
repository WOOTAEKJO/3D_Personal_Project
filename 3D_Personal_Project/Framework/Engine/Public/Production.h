#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameInstance;
class CGameObject;

class ENGINE_DLL CProduction abstract : public CBase
{
public:
	typedef struct tagFontInfo
	{
		_uint	iFontTag;
		wstring strText;
		_float2 vPosition;
		_float4 vColor = { 1.f,1.f,1.f,1.f };
		_float	fScale = 1.f;
		_float2 vOrigin = { 0.f,0.f };
		_float fRotation = 0.f;

	}PRODUCTION_DIALOG_DESC;

protected:
	CProduction();
	virtual	~CProduction() = default;

public:
	virtual HRESULT	Initialize();
	virtual	void	Enter();
	virtual	void	Tick();
	virtual	void	Render();
	virtual	void	Exite();
	
	HRESULT	Add_Actor(const wstring& strActorTag, CGameObject* pActor);
	HRESULT	Add_Dialog(const wstring& strDialogTag, PRODUCTION_DIALOG_DESC Dialog);

	void	Render_Dialog(const wstring& strDialogTag);

protected:
	CGameInstance* m_pGameInstance = { nullptr };

protected:
	map<wstring, CGameObject*> m_mapActor;	// 배우들
	map<wstring, PRODUCTION_DIALOG_DESC> m_mapDialogue; // 대사들

protected:
	CGameObject*	Find_Actor(const wstring& strActorTag);
	PRODUCTION_DIALOG_DESC* Find_Dialog(const wstring& strDialogTag);
public:
	virtual	void	Free();
};

END

// 그냥 상태 같은데
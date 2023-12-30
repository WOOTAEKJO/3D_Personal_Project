#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CDataMgr final : public CBase
{
	DECLARE_SINGLETON(CDataMgr)
private:
	CDataMgr();
	virtual	~CDataMgr() = default;

public:
	HRESULT		Initialize();

public:
	HRESULT		Level_Object_Load(const _char* strFilePath);

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual	void	Free() override;
};

END
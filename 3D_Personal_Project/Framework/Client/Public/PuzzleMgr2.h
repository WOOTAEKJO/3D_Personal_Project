#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;

END

BEGIN(Client)

class CPuzzleMgr2 final : public CBase
{
	//DECLARE_SINGLETON(CPuzzleMgr2);
private:
	CPuzzleMgr2();
	virtual	~CPuzzleMgr2() = default;

public:
	HRESULT	Initialize();

private:

public:
	virtual void	Free() override;

};

END
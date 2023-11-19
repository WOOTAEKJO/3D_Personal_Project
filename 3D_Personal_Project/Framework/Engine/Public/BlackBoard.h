#pragma once
#include "Base.h"

BEGIN(Engine)

class CBlackBoard final : public CBase
{
private:
	CBlackBoard();
	virtual	~CBlackBoard() = default;

public:
	HRESULT	Initialize();

private:

public:
	static	CBlackBoard* Create();
	virtual	void	Free() override;
};

END
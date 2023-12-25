#pragma once
#include "Client_Defines.h"
#include "Leaf.h"

BEGIN(Client)

class CAI_Normal_Monster_IDLE final : CLeaf
{
private:
	CAI_Normal_Monster_IDLE();
	CAI_Normal_Monster_IDLE(class CBlackBoard* pBlackBoard);
	virtual	~CAI_Normal_Monster_IDLE() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:


public:
	static	CAI_Normal_Monster_IDLE* Create();
	virtual	void	Free() override;
};

END
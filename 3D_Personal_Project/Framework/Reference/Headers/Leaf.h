#pragma once
#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CLeaf abstract : public CNode
{
protected:
	CLeaf();
	CLeaf(class CBlackBoard* pBlackBoard);
	virtual	~CLeaf() = default;

public:
	virtual	HRESULT	Initialize() = 0;
	virtual	STATUS	Update() = 0;
	virtual	HRESULT	Terminate(STATUS eStatus) = 0;

protected:
	

public:
	virtual	void	Free() override;
};

END
#pragma once
#include "Node.h"

BEGIN(Engine)

class CLeaf abstract : public CNode
{
protected:
	CLeaf();
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
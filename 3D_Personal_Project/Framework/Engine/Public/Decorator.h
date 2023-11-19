#pragma once
#include "Node.h"

BEGIN(Engine)

class CDecorator abstract : public CNode
{
protected:
	CDecorator();
	virtual	~CDecorator() = default;

public:
	virtual	HRESULT	Initialize() = 0;
	virtual	STATUS	Update() = 0;
	virtual	HRESULT	Terminate(STATUS eStatus) = 0;

public:
	HRESULT	Add_Child(CNode* pChild);

protected:
	CNode*		m_pChild = { nullptr };

public:
	virtual	void	Free() override;
};

END
#pragma once
#include "Node.h"

BEGIN(Engine)

class CComposite abstract : public CNode
{
protected:
	CComposite();
	virtual	~CComposite() = default;

public:
	virtual	HRESULT	Initialize() = 0;
	virtual	STATUS	Update() = 0;
	virtual	HRESULT	Terminate(STATUS eStatus) = 0;

public:
	HRESULT	Add_Children(CNode* pChildNode);

protected:
	vector<CNode*> m_vecChildren;
	vector<CNode*>::iterator	m_iter;

public:
	virtual	void	Free() override;
};

END
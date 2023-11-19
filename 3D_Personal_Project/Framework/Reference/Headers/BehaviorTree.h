#pragma once
#include "Node.h"

BEGIN(Engine)

class CBehaviorTree final : public CNode
{
private:
	CBehaviorTree();
	virtual	~CBehaviorTree() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

private:
	CNode*	m_pRoot = { nullptr };

public:
	static	CBehaviorTree*	Create();
	virtual	void			Free() override;
};

END
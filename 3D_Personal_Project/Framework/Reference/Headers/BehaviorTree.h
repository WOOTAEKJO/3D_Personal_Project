#pragma once
#include "Node.h"

BEGIN(Engine)

class CBehaviorTree final : public CNode
{
private:
	CBehaviorTree();
	CBehaviorTree(CNode* pRoot);
	virtual	~CBehaviorTree() = default;

public:
	virtual	HRESULT	Initialize() override;
	virtual	STATUS	Update() override;
	virtual	HRESULT	Terminate(STATUS eStatus) override;

public:
	void	Set_Root(CNode* pRoot) { m_pRoot = pRoot; }

private:
	CNode*	m_pRoot = { nullptr };

public:
	static	CBehaviorTree*	Create();
	virtual	void			Free() override;
};

END
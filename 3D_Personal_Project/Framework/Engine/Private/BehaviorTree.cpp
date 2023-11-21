#include "..\Public\BehaviorTree.h"
#include "BlackBoard.h"

CBehaviorTree::CBehaviorTree()
{
	m_pBlackBoard = CBlackBoard::Create();
}

CBehaviorTree::CBehaviorTree(CNode* pRoot)
	:m_pRoot(pRoot)
{
	Safe_AddRef(m_pRoot);
}

HRESULT CBehaviorTree::Initialize()
{
	return S_OK;
}

CNode::STATUS CBehaviorTree::Update()
{
	return m_pRoot->Tick();
}

HRESULT CBehaviorTree::Terminate(STATUS eStatus)
{
	return S_OK;
}

CBehaviorTree* CBehaviorTree::Create()
{
	CBehaviorTree* pInstance = new CBehaviorTree();

	/*if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CBehaviorTree");
		Safe_Release(pInstance);
	}*/

	return pInstance;
}

void CBehaviorTree::Free()
{
	__super::Free();
	Safe_Release(m_pRoot);
}

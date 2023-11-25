#include "..\Public\Builder.h"
#include "BehaviorTree.h"

CBuilder::CBuilder()
{
    m_pTree = CBehaviorTree::Create();
}
CBuilder::~CBuilder()
{
    //Safe_Release(m_pRoot);
}

CNode* CBuilder::Build()
{
    if (m_pTree == nullptr)
        return nullptr;

    m_pTree->Set_Root(m_pRoot);

    return m_pTree;
}


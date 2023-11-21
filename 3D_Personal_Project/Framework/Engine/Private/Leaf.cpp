#include "..\Public\Leaf.h"

CLeaf::CLeaf()
{
}

CLeaf::CLeaf(CBlackBoard* pBlackBoard)
	:CNode(pBlackBoard)
{
}

void CLeaf::Free()
{
	__super::Free();
	
}

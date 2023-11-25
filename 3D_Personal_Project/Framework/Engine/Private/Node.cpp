#include "..\Public\Node.h"
#include "BlackBoard.h"

CNode::CNode()
{
}

CNode::CNode(CBlackBoard* pBlackBoard)
	:m_pBlackBoard(pBlackBoard)
{
	//Safe_AddRef(m_pBlackBoard);
}

CNode::STATUS CNode::Tick()
{
	if (m_eStatus != STATUS::RUNNING)
	{
		Initialize();
	}

	m_eStatus = Update();

	if (m_eStatus != STATUS::RUNNING)
	{
		Terminate(m_eStatus);
	}

	return m_eStatus;
}

void	CNode::Set_BlackBoard(class CBlackBoard* pBlackBoard)
{
	if (pBlackBoard == nullptr)
		return;

	m_pBlackBoard = pBlackBoard;
	Safe_AddRef(m_pBlackBoard);
}

void CNode::Free()
{
	__super::Free();

	Safe_Release(m_pBlackBoard);
}

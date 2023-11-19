#include "..\Public\Node.h"
#include "BlackBoard.h"

CNode::CNode()
{
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

void CNode::Free()
{
	__super::Free();

	Safe_Release(m_pBlackBoard);
}

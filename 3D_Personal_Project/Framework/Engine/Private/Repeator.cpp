#include "..\Public\Repeator.h"

CRepeator::CRepeator()
{
}

CRepeator::CRepeator(_uint iLimit)
	:m_iLimit(iLimit)
{
}

HRESULT CRepeator::Initialize()
{
	m_iCount = 0;

	return S_OK;
}

CNode::STATUS CRepeator::Update()
{
	m_pChild->Tick();

	if (m_iLimit > 0 && ++m_iCount == m_iLimit)
		return STATUS::SUCCESS;

	return STATUS::RUNNING;
}

HRESULT CRepeator::Terminate(STATUS eStatus)
{
	return S_OK;
}

CRepeator* CRepeator::Create()
{
	return new CRepeator();
}

void CRepeator::Free()
{
	__super::Free();
}

#include "..\Public\Sequence.h"

CSequence::CSequence()
{
}

HRESULT CSequence::Initialize()
{
    m_iter = m_vecChildren.begin();

    return S_OK;
}

CNode::STATUS CSequence::Update()
{
    if (m_vecChildren.empty())
        return STATUS::FAILURE;

    while (m_iter != m_vecChildren.end())
    {
        auto Status = (*m_iter)->Tick();

        if (Status != STATUS::SUCCESS)
            return Status;

        m_iter++;
    }

    return STATUS::SUCCESS;
}

HRESULT CSequence::Terminate(STATUS eStatus)
{
    return S_OK;
}

CSequence* CSequence::Create()
{
    return new CSequence();
}

void CSequence::Free()
{
    __super::Free();
}

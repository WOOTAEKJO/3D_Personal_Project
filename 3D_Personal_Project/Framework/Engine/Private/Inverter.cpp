#include "..\Public\Inverter.h"

CInverter::CInverter()
{
}

HRESULT CInverter::Initialize()
{
    return S_OK;
}

CNode::STATUS CInverter::Update()
{
    auto Status = m_pChild->Tick();

    if (Status = STATUS::SUCCESS) {
        return STATUS::FAILURE;
    }
    else if (Status = STATUS::FAILURE) {
        return STATUS::SUCCESS;
    }

    return Status;
}

HRESULT CInverter::Terminate(STATUS eStatus)
{
    return S_OK;
}

CInverter* CInverter::Create()
{
    return new CInverter();
}

void CInverter::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "..\Public\AI_Normal_Monster_IDLE.h"

CAI_Normal_Monster_IDLE::CAI_Normal_Monster_IDLE()
{
}

CAI_Normal_Monster_IDLE::CAI_Normal_Monster_IDLE(CBlackBoard* pBlackBoard)
{
}

HRESULT CAI_Normal_Monster_IDLE::Initialize()
{
    return S_OK;
}

CLeaf::STATUS CAI_Normal_Monster_IDLE::Update()
{
    return STATUS();
}

HRESULT CAI_Normal_Monster_IDLE::Terminate(STATUS eStatus)
{
    return S_OK;
}

CAI_Normal_Monster_IDLE* CAI_Normal_Monster_IDLE::Create()
{
    return new CAI_Normal_Monster_IDLE;
}

void CAI_Normal_Monster_IDLE::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "..\Public\TestAction.h"

CTestAction::CTestAction()
{
}

HRESULT CTestAction::Initialize()
{
	return S_OK;
}

CNode::STATUS CTestAction::Update()
{
	return STATUS::SUCCESS;
}

HRESULT CTestAction::Terminate(STATUS eStatus)
{
	return S_OK;
}

CTestAction* CTestAction::Create()
{
	return new CTestAction();
}

void CTestAction::Free()
{
	__super::Free();
}

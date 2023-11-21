#include "..\Public\Failer.h"

CFailer::CFailer()
{
}

HRESULT CFailer::Initialize()
{
	return S_OK;
}

CNode::STATUS CFailer::Update()
{
	m_pChild->Tick();

	return STATUS::FAILURE;
}

HRESULT CFailer::Terminate(STATUS eStatus)
{
	return S_OK;
}

CFailer* CFailer::Create()
{
	return new CFailer();
}

void CFailer::Free()
{
	__super::Free();
}

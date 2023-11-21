#include "..\Public\Succeeder.h"

CSucceeder::CSucceeder()
{
}

HRESULT CSucceeder::Initialize()
{
	return S_OK;
}

CNode::STATUS CSucceeder::Update()
{
	m_pChild->Tick();

	return STATUS::SUCCESS;
}

HRESULT CSucceeder::Terminate(STATUS eStatus)
{
	return S_OK;
}

CSucceeder* CSucceeder::Create()
{
	return new CSucceeder();
}

void CSucceeder::Free()
{
	__super::Free();
}

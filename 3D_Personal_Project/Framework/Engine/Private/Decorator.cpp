#include "..\Public\Decorator.h"

CDecorator::CDecorator()
{
}

HRESULT CDecorator::Add_Child(CNode* pChild)
{
	return S_OK;
}

void CDecorator::Free()
{
	__super::Free();
}

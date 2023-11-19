#include "..\Public\AICom.h"

CAICom::CAICom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CAICom::CAICom(const CAICom& rhs)
	:CComponent(rhs)
{
}

HRESULT CAICom::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CAICom::Initialize(void* pArg)
{
	return S_OK;
}

CAICom* CAICom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAICom* pInstance = new CAICom(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CAICom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAICom::Clone(void* pArg)
{
	CAICom* pInstance = new CAICom(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CAICom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAICom::Free()
{
	__super::Free();
}

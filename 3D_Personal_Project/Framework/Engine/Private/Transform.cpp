#include "..\Public\Transform.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

HRESULT CTransform::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	return S_OK;
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	return nullptr;
}

void CTransform::Free()
{
	__super::Free();
}

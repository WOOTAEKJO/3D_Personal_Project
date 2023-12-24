#include "..\Public\Bounding_OBB.h"

#include "DebugDraw.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_OBB::Initialize(BOUNDING_DESC* Bounding_Desc)
{
	BOUNDING_OBB_DESC* OBB_Desc = (BOUNDING_OBB_DESC*)(Bounding_Desc);

	_float4 vQuaternion = {};

	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(OBB_Desc->vRotation.x, OBB_Desc->vRotation.y, OBB_Desc->vRotation.z));

	m_pOBB = new BoundingOrientedBox(OBB_Desc->vCenter, OBB_Desc->vExtents, vQuaternion);
	if (m_pOBB == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	if (m_pOBB == nullptr || pBatch == nullptr)
		return E_FAIL;

	_vector vColor = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DX::Draw(pBatch, *m_pOBB, vColor);

	return S_OK;
}

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc)
{
	CBounding_OBB* pInstance = new CBounding_OBB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Bounding_Desc))) {
		MSG_BOX("Failed to Created : CBounding_OBB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_OBB::Free()
{
	__super::Free();

	Safe_Delete(m_pOBB);
}

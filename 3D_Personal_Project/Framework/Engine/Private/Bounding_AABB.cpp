#include "..\Public\Bounding_AABB.h"

#include "DebugDraw.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_AABB::Initialize(BOUNDING_DESC* Bounding_Desc)
{
	BOUNDING_AABB_DESC* AABB_Desc = (BOUNDING_AABB_DESC*)(Bounding_Desc);

	m_pAABB = new BoundingBox(AABB_Desc->vCenter, AABB_Desc->vExtents);
	if (m_pAABB == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	if (m_pAABB == nullptr || pBatch == nullptr)
		return E_FAIL;

	_vector vColor = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DX::Draw(pBatch, *m_pAABB, vColor);

	return S_OK;
}

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc)
{
	CBounding_AABB* pInstance = new CBounding_AABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Bounding_Desc))) {
		MSG_BOX("Failed to Created : CBounding_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pAABB);
}

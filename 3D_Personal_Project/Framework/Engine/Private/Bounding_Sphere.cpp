#include "..\Public\Bounding_Sphere.h"

#include "DebugDraw.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_Sphere::Initialize(BOUNDING_DESC* Bounding_Desc)
{
	BOUNDING_SPHERE_DESC* Sphere_Desc = (BOUNDING_SPHERE_DESC*)(Bounding_Desc);

	m_pSphere = new BoundingSphere(Sphere_Desc->vCenter, Sphere_Desc->fRadius);
	if (m_pSphere == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	if (m_pSphere == nullptr || pBatch == nullptr)
		return E_FAIL;

	_vector vColor = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	DX::Draw(pBatch, *m_pSphere, vColor);

	return S_OK;
}

CBounding_Sphere* CBounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc)
{
	CBounding_Sphere* pInstance = new CBounding_Sphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Bounding_Desc))) {
		MSG_BOX("Failed to Created : CBounding_Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_Sphere::Free()
{
	__super::Free();

	Safe_Delete(m_pSphere);
}

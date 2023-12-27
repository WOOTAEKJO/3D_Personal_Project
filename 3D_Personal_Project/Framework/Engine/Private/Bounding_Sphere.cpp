#include "..\Public\Bounding_Sphere.h"

#include "DebugDraw.h"
#include "Collider.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_Sphere::Initialize(BOUNDING_DESC* Bounding_Desc)
{
	BOUNDING_SPHERE_DESC* Sphere_Desc = (BOUNDING_SPHERE_DESC*)(Bounding_Desc);

	m_eType = Sphere_Desc->eType;

	m_pOriSphere = new BoundingSphere(Sphere_Desc->vCenter, Sphere_Desc->fRadius);
	if (m_pOriSphere == nullptr)
		return E_FAIL;

	m_pSphere = new BoundingSphere(*m_pOriSphere);
	if (m_pSphere == nullptr)
		return E_FAIL;

	return S_OK;
}

void CBounding_Sphere::Update(_fmatrix matWorld)
{
	m_pOriSphere->Transform(*m_pSphere, matWorld);
}

HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	if (m_pSphere == nullptr || pBatch == nullptr)
		return E_FAIL;

	DX::Draw(pBatch, *m_pSphere, vColor);

	return S_OK;
}

_bool CBounding_Sphere::Collision(CCollider* pTargetCollider)
{
	CBounding* pBounding = pTargetCollider->Get_Bounding();

	switch (pBounding->Get_ColisionType())
	{
	case TYPE::TYPE_AABB:
		return m_pSphere->Intersects(*dynamic_cast<CBounding_AABB*>(pBounding)->Get_BoundingAABB());
		break;
	case TYPE::TYPE_OBB:
		return m_pSphere->Intersects(*dynamic_cast<CBounding_OBB*>(pBounding)->Get_BoundingOBB());
		break;
	case TYPE::TYPE_SPHERE:
		return m_pSphere->Intersects(*dynamic_cast<CBounding_Sphere*>(pBounding)->Get_BoundingSphere());
		break;
	}
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
	Safe_Delete(m_pOriSphere);
}

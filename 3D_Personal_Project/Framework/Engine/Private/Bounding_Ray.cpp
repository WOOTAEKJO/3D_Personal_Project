#include "..\Public\Bounding_Ray.h"

#include "DebugDraw.h"
#include "Collider.h"

CBounding_Ray::CBounding_Ray(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_Ray::Initialize(BOUNDING_DESC* Bounding_Desc)
{
	BOUNDING_RAY_DESC* Ray_Desc = (BOUNDING_RAY_DESC*)(Bounding_Desc);

	m_eType = Ray_Desc->eType;

	m_OriginRay.vCenter = Ray_Desc->vCenter;
	m_OriginRay.vDir = Ray_Desc->vDir;

	m_Ray = m_OriginRay;

	return S_OK;
}

void CBounding_Ray::Update(_fmatrix matWorld)
{
	XMStoreFloat4x4(&m_matWorld, matWorld);
	XMStoreFloat3(&m_Ray.vCenter ,XMVector3TransformCoord(XMLoadFloat3(&m_OriginRay.vCenter), matWorld));
	XMStoreFloat3(&m_Ray.vDir, XMVector3Normalize( XMVector3TransformNormal(XMLoadFloat3(&m_OriginRay.vDir), matWorld)));
}

HRESULT CBounding_Ray::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	if (pBatch == nullptr)
		return E_FAIL;

	DX::DrawRay(pBatch,XMLoadFloat3(&m_Ray.vCenter), XMLoadFloat3(&m_Ray.vDir) * 10.f,false,vColor);

	return S_OK;
}

_bool CBounding_Ray::Collision(CCollider* pTargetCollider, _float3* vCollisionDir,
	_float* fPushedDist, _float4* vColliderPos)
{
	vector<CBounding*> vecBounding = pTargetCollider->Get_BoundingVec();
	_float	fDist = 0.f;

	for (auto& iter : vecBounding) {
		switch (iter->Get_ColisionType())
		{
		case TYPE::TYPE_AABB:
			return dynamic_cast<CBounding_AABB*>(iter)->Get_BoundingAABB()->
				Intersects(XMLoadFloat3(&m_Ray.vCenter), XMLoadFloat3(&m_Ray.vDir), fDist);
			break;
		case TYPE::TYPE_OBB:
			return dynamic_cast<CBounding_OBB*>(iter)->Get_BoundingOBB()->
				Intersects(XMLoadFloat3(&m_Ray.vCenter), XMLoadFloat3(&m_Ray.vDir), fDist);
			break;
		case TYPE::TYPE_SPHERE:
			return dynamic_cast<CBounding_Sphere*>(iter)->Get_BoundingSphere()
				->Intersects(XMLoadFloat3(&m_Ray.vCenter), XMLoadFloat3(&m_Ray.vDir), fDist);
			break;
		}
	}

	return false;
}

CBounding_Ray* CBounding_Ray::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOUNDING_DESC* Bounding_Desc)
{
	CBounding_Ray* pInstance = new CBounding_Ray(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Bounding_Desc))) {
		MSG_BOX("Failed to Created : CBounding_Ray");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_Ray::Free()
{
	__super::Free();
}

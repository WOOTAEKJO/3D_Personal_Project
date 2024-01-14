#include "..\Public\Bounding_AABB.h"

#include "DebugDraw.h"
#include "Collider.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_AABB::Initialize(BOUNDING_DESC* Bounding_Desc)
{
	BOUNDING_AABB_DESC* AABB_Desc = (BOUNDING_AABB_DESC*)(Bounding_Desc);
	
	m_eType = AABB_Desc->eType;

	m_pOriAABB = new BoundingBox(AABB_Desc->vCenter, AABB_Desc->vExtents);
	if (m_pOriAABB == nullptr)
		return E_FAIL;

	m_pAABB = new BoundingBox(*m_pOriAABB);
	if (m_pAABB == nullptr)
		return E_FAIL;

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix matWorld)
{
	_matrix WorldMat = matWorld;

	WorldMat.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(WorldMat.r[0]));
	WorldMat.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(WorldMat.r[1]));
	WorldMat.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(WorldMat.r[2]));

	m_pOriAABB->Transform(*m_pAABB, WorldMat);
}

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	if (m_pAABB == nullptr || pBatch == nullptr)
		return E_FAIL;

	DX::Draw(pBatch, *m_pAABB, vColor);

	return S_OK;
}

_bool CBounding_AABB::Collision(CCollider* pTargetCollider, _float3* vCollisionDir, _float* fPushedDist)
{
	//CBounding* pBounding = pTargetCollider->Get_Bounding();
	vector<CBounding*> vecBounding = pTargetCollider->Get_BoundingVec();

	for (auto& iter : vecBounding) {
		switch (iter->Get_ColisionType())
		{
		case TYPE::TYPE_AABB:
			return m_pAABB->Intersects(*dynamic_cast<CBounding_AABB*>(iter)->Get_BoundingAABB());
			break;
		case TYPE::TYPE_OBB:
			return m_pAABB->Intersects(*dynamic_cast<CBounding_OBB*>(iter)->Get_BoundingOBB());
			break;
		case TYPE::TYPE_SPHERE:
			return m_pAABB->Intersects(*dynamic_cast<CBounding_Sphere*>(iter)->Get_BoundingSphere());
			break;
		}
	}

	return false;
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
	Safe_Delete(m_pOriAABB);
}

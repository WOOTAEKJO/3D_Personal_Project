#include "..\Public\Bounding_OBB.h"

#include "DebugDraw.h"
#include "Collider.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding(pDevice, pContext)
{
}

HRESULT CBounding_OBB::Initialize(BOUNDING_DESC* Bounding_Desc)
{
	BOUNDING_OBB_DESC* OBB_Desc = (BOUNDING_OBB_DESC*)(Bounding_Desc);

	m_eType = OBB_Desc->eType;

	_float4 vQuaternion = {};

	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(OBB_Desc->vRotation.x, OBB_Desc->vRotation.y, OBB_Desc->vRotation.z));

	m_pOriOBB = new BoundingOrientedBox(OBB_Desc->vCenter, OBB_Desc->vExtents, vQuaternion);
	if (m_pOriOBB == nullptr)
		return E_FAIL;

	m_pOBB = new BoundingOrientedBox(*m_pOriOBB);
	if (m_pOBB == nullptr)
		return E_FAIL;

	return S_OK;
}

void CBounding_OBB::Update(_fmatrix matWorld)
{
	XMStoreFloat4x4(&m_matWorld, matWorld);
	m_pOriOBB->Transform(*m_pOBB, matWorld);
}

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	if (m_pOBB == nullptr || pBatch == nullptr)
		return E_FAIL;

	DX::Draw(pBatch, *m_pOBB, vColor);

	return S_OK;
}

_bool CBounding_OBB::Collision(CCollider* pTargetCollider, _float3* vCollisionDir, _float* fPushedDist)
{
	//CBounding* pBounding = pTargetCollider->Get_Bounding();

	vector<CBounding*> vecBounding = pTargetCollider->Get_BoundingVec();

	for (auto& iter : vecBounding) {
		switch (iter->Get_ColisionType())
		{
		case TYPE::TYPE_AABB:
			return m_pOBB->Intersects(*dynamic_cast<CBounding_AABB*>(iter)->Get_BoundingAABB());
			break;
		case TYPE::TYPE_OBB:
			return m_pOBB->Intersects(*dynamic_cast<CBounding_OBB*>(iter)->Get_BoundingOBB());
			break;
		case TYPE::TYPE_SPHERE:
			return m_pOBB->Intersects(*dynamic_cast<CBounding_Sphere*>(iter)->Get_BoundingSphere());
			break;
		}
	}

	return false;
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
	Safe_Delete(m_pOriOBB);
}

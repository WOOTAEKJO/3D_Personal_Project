#include "..\Public\RigidBody.h"

#include "GameObject.h"
#include "Transform.h"

CRigidBody::CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CRigidBody::CRigidBody(const CRigidBody& rhs)
	:CComponent(rhs)
{
}

HRESULT CRigidBody::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CRigidBody::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	m_vMaxSpeed = _float3(100.f,100.f,100.f);
	m_vMinSpeed = _float3(-100.f, -100.f, -100.f);
	m_vResist = _float3(0.001f, 0.1f, 0.001f);

	m_pOwner = ((RIGIDBODY_DESC*)pArg)->pOwner;

	return S_OK;
}

void CRigidBody::Priority_Tick(_float fTimeDelta)
{
}

void CRigidBody::Tick(_float fTimeDelta)
{
	_bool bIsOnGround = m_pOwner->Get_WorldMatrix().m[3][1] <= 0.f;

	if (m_bGravity)
	{
		if (bIsOnGround) {
			m_bJump = false;
			m_bGravity = false;
			
		}
		else {
			Force(XMVectorSet(0.f, -1.f, 0.f, 0.f), m_fGravity, fTimeDelta);
		}
	}

	m_vVelocity.x *= powf(m_vResist.x, fTimeDelta);
	m_vVelocity.y *= powf(m_vResist.y, fTimeDelta);
	m_vVelocity.z *= powf(m_vResist.z, fTimeDelta);

	/*XMStoreFloat3(&m_vVelocity, XMVectorClamp(XMLoadFloat3(&m_vVelocity),
		XMLoadFloat3(&m_vMinSpeed), XMLoadFloat3(&m_vMaxSpeed)));*/

	m_pOwner->Get_Transform()->Translate(XMLoadFloat3(&m_vVelocity));
}

void CRigidBody::Late_Tick(_float fTimeDelta)
{
	if (XMVector3NearEqual(XMVectorZero(), XMLoadFloat3(&m_vVelocity), XMVectorSet(0.2f, 0.2f, 0.2f, 0.f)))
	{
		m_vVelocity = _float3(0.f, 0.f, 0.f);

		if (m_bJump)
			m_bGravity = true;
	}
	
}

void CRigidBody::Force(_fvector vDir, _float fPower, _float fTimeDelta)
{
	XMStoreFloat3(&m_vVelocity,XMLoadFloat3(&m_vVelocity) + XMVector3Normalize(vDir) * fPower * fTimeDelta);
}

CRigidBody* CRigidBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRigidBody* pInstance = new CRigidBody(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CRigidBody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRigidBody::Clone(void* pArg)
{
	CRigidBody* pInstance = new CRigidBody(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CRigidBody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRigidBody::Free()
{
	//Safe_Release(m_pOwner);
	__super::Free();
	
}

#include "..\Public\RigidBody.h"

#include "GameObject.h"
#include "Transform.h"
#include "Navigation.h"

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
	m_vResist = _float3(0.2f, 0.2f, 0.2f);

	m_pOwner = ((RIGIDBODY_DESC*)pArg)->pOwner;

	return S_OK;
}

void CRigidBody::Priority_Tick(_float fTimeDelta)
{
}

void CRigidBody::Tick(_float fTimeDelta)
{
	
	if(m_bGravity)
		Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fGravity * fTimeDelta, TYPE_VELOCITY);

	XMStoreFloat3(&m_vPower[TYPE_VELOCITY], XMLoadFloat3(&m_vPower[TYPE_VELOCITY]) +
		XMLoadFloat3(&m_vPower[TYPE_ACCEL]) * fTimeDelta);

	Resistance(TYPE_VELOCITY,fTimeDelta);

	Update_Transform(TYPE_VELOCITY,fTimeDelta);
}

void CRigidBody::Late_Tick(_float fTimeDelta)
{
	Near_Zero_Force(TYPE_VELOCITY);
}

void CRigidBody::Jump(_float fJumpPower, _float fGravityPower)
{
	m_bGravity = true;
	m_fGravity = fGravityPower;
	Force(XMVectorSet(0.f, 1.f, 0.f, 0.f), fJumpPower,TYPE_VELOCITY);
}

_bool CRigidBody::Is_Land()
{
	if (m_pOwner->Get_Transform()->Get_WorldMatrix_Float4x4().m[3][1] <= 0.f)
		return true;

	return false;
}

void CRigidBody::Land()
{
	m_bGravity = false;

	Reset_Force(TYPE_VELOCITY);

	if (m_pOwner->Get_Transform()->Get_WorldMatrix_Float4x4().m[3][1] < 0.f) {

		_float4 vPos;
		memcpy(&vPos, &m_pOwner->Get_Transform()->Get_WorldMatrix_Float4x4().m[3], sizeof(_float4));

		m_pOwner->Get_Transform()->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(vPos.x, 0.f, vPos.z, vPos.w));
	}
}

_bool CRigidBody::Is_Power_Zero(TYPE eType)
{
	if (XMVector3Equal(XMLoadFloat3(&m_vPower[eType]), XMVectorZero()))
		return true;

	return false;
}

void CRigidBody::Near_Zero_Force(TYPE eType)
{
	if (XMVector3NearEqual(XMVectorZero(), XMLoadFloat3(&m_vPower[eType]), XMVectorSet(0.1f, 0.1f, 0.1f, 0.f)))
	{
		m_vPower[eType] = _float3(0.f, 0.f, 0.f);
	}
}

void CRigidBody::Force(_fvector vDir, _float fPower, TYPE eType)
{
	XMStoreFloat3(&m_vPower[eType],XMLoadFloat3(&m_vPower[eType]) + XMVector3Normalize(vDir) * fPower);
}

void CRigidBody::Reset_Force(TYPE eType)
{
	m_vPower[eType] = _float3(0.f, 0.f, 0.f);
}

void CRigidBody::Update_Transform(TYPE eType, _float fTimeDelta)
{
	m_pOwner->Get_Transform()->Translate(XMLoadFloat3(&m_vPower[eType])* fTimeDelta,
		dynamic_cast<CNavigation*>( m_pOwner->Get_Component(TAG_NAVIGATION)));
}

void CRigidBody::Resistance(TYPE eType,_float fTimeDelta)
{
	m_vPower[eType].x *= powf(m_vResist.x, fTimeDelta);
	m_vPower[eType].y *= powf(m_vResist.y, fTimeDelta);
	m_vPower[eType].z *= powf(m_vResist.z, fTimeDelta);
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

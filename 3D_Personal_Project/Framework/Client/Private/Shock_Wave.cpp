#include "stdafx.h"
#include "..\Public\Shock_Wave.h"

#include "GameInstance.h"
#include "Character.h"

CShock_Wave::CShock_Wave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CShock_Wave::CShock_Wave(const CShock_Wave& rhs)
	: CBullet(rhs)
{
}

HRESULT CShock_Wave::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShock_Wave::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS) -
		m_pOwner->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS);

	XMStoreFloat3(&m_vLook, vLook);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;

	return S_OK;
}

void CShock_Wave::Priority_Tick(_float fTimeDelta)
{
	Update_MatWorlds(fTimeDelta);

	//m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());
	for (_uint i = 0; i < 20; i++)
	{
		m_pColliderCom->Update_Each(i, XMLoadFloat4x4(&m_matWorlds[i]));
	}

	//__super::Priority_Tick(fTimeDelta);
}

void CShock_Wave::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
}

void CShock_Wave::Late_Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > m_fLifeTime)
		Set_Dead();

	__super::Late_Tick(fTimeDelta);

}

HRESULT CShock_Wave::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShock_Wave::Bind_ShaderResources()
{
	
	return S_OK;
}

HRESULT CShock_Wave::Ready_Component()
{
	_float3 vCenter = _float3(0.f, m_fRadius, 0.f);

	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.bUseCol = true;
	Sphere_Desc.fRadius = m_fRadius;
	Sphere_Desc.vCenter = vCenter;

	Create_MatWorlds();

	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;
	for (_uint i = 1; i < 20; i++)
	{
		_matrix matRot = XMMatrixRotationY(XMConvertToRadians(18.f));
		_vector vRotLook = XMVector3TransformNormal(XMLoadFloat3(&m_vLook), matRot);
		
		_vector vDir = (vRotLook) - (XMLoadFloat3(&m_vLook));
		XMStoreFloat3(&m_vLook, vRotLook);

		_vector vPos;
		memcpy(&vPos, &m_matWorlds[i - 1].m[3], sizeof(_float4));
		vPos += vDir;

		memcpy(&m_matWorlds[i].m[3], &vPos, sizeof(_float4));

		m_pColliderCom->Add_Bounding(&Sphere_Desc);
	}

	return S_OK;
}

void CShock_Wave::Toward(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMLoadFloat3(&m_vLook);

	vPos = vPos + vLook * fTimeDelta * m_fSpeed;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
}

void CShock_Wave::Update_MatWorlds(_float fTimeDelta)
{
	for (_uint i = 0; i < 20; i++)
	{
		_vector vPos, vLook;
		memcpy(&vPos, &m_matWorlds[i].m[3], sizeof(_float4));
		memcpy(&vLook, &m_matWorlds[i].m[2], sizeof(_float4));

		vPos = vPos + XMVector3Normalize(vLook) * fTimeDelta * m_fSpeed;

		memcpy(&m_matWorlds[i].m[3], &vPos, sizeof(_float4));
	}
}

void CShock_Wave::Create_MatWorlds()
{
	for (_uint i = 0; i < 20; i++)
	{
		if (i == 0)
		{
			m_matWorlds[i] = m_pTransformCom->Get_WorldMatrix_Float4x4();
		}
		else
		{
			_float4 vTmp;
			_matrix matRot = XMMatrixRotationY(XMConvertToRadians(18.f));
			memcpy(&vTmp, &m_matWorlds[i - 1].m[0], sizeof(_float4));
			_vector vRight = XMVector3TransformNormal(XMLoadFloat4(&vTmp), matRot);

			memcpy(&vTmp, &m_matWorlds[i - 1].m[2], sizeof(_float4));
			_vector vLook = XMVector3TransformNormal(XMLoadFloat4(&vTmp), matRot);

			memcpy(&m_matWorlds[i].m[0], &vRight, sizeof(_float4));
			memcpy(&m_matWorlds[i].m[1], &m_matWorlds[i - 1].m[1], sizeof(_float4));
			memcpy(&m_matWorlds[i].m[2], &vLook, sizeof(_float4));

		}
	}
}

CShock_Wave* CShock_Wave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShock_Wave* pInstance = new CShock_Wave(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShock_Wave");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CShock_Wave::Clone(void* pArg)
{
	CShock_Wave* pInstance = new CShock_Wave(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShock_Wave");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShock_Wave::Free()
{
	__super::Free();
}

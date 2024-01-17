#include "stdafx.h"
#include "..\Public\Laser.h"

#include "GameInstance.h"
#include "Character.h"

CLaser::CLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CLaser::CLaser(const CLaser& rhs)
	: CBullet(rhs)
{
}

HRESULT CLaser::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLaser::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;

	XMStoreFloat4(&m_vTargetPos, m_pTarget->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS));

	return S_OK;
}

void CLaser::Priority_Tick(_float fTimeDelta)
{
	Update_MatWorlds(fTimeDelta);
}

void CLaser::Tick(_float fTimeDelta)
{
	
	__super::Tick(fTimeDelta);
}

void CLaser::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CLaser::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLaser::Bind_ShaderResources()
{
	
	return S_OK;
}

HRESULT CLaser::Ready_Component()
{
	CBounding_Ray::BOUNDING_RAY_DESC Ray_Desc = {};
	Ray_Desc.pOnwer = this;
	Ray_Desc.eType = CBounding::TYPE::TYPE_RAY;
	Ray_Desc.bUseCol = true;
	Ray_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	XMStoreFloat3(&Ray_Desc.vDir, XMVectorSet(0.f,0.f,1.f,0.f));

	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Ray_Desc))) return E_FAIL;

	return S_OK;
}

void CLaser::Update_MatWorlds(_float fTimeDelta)
{
	_vector vTargetPos = m_pTarget->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS);

	_vector vDir = XMVector3Normalize( vTargetPos - XMLoadFloat4(&m_vTargetPos));

	XMStoreFloat4(&m_vTargetPos, XMLoadFloat4(&m_vTargetPos) + vDir * fTimeDelta * 1.5f);
	
	m_pTransformCom->LookAt(XMLoadFloat4(&m_vTargetPos));
	
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());
}

void CLaser::Update_TartgetPos()
{
}

CLaser* CLaser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLaser* pInstance = new CLaser(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLaser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLaser::Clone(void* pArg)
{
	CLaser* pInstance = new CLaser(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLaser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLaser::Free()
{
	__super::Free();
}

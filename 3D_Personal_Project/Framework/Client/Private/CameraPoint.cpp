#include "stdafx.h"
#include "..\Public\CameraPoint.h"

#include "GameInstance.h"

CCameraPoint::CCameraPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CCameraPoint::CCameraPoint(const CCameraPoint& rhs)
	:CGameObject(rhs)
{
}

HRESULT CCameraPoint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCameraPoint::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CAMERAPOINT_DESC* pTriggerDesc = (CAMERAPOINT_DESC*)pArg;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_TRIGGER, m_pColliderCom)))
		return E_FAIL;

	_vector vPos = XMLoadFloat4(&pTriggerDesc->vPosition);
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	m_pTransformCom->Set_Scaling(pTriggerDesc->vScale.x, pTriggerDesc->vScale.y, pTriggerDesc->vScale.z);

	m_pTransformCom->LookAt(XMLoadFloat4(&pTriggerDesc->vAtPos));

	if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_BOSS1)
	{
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("Boss2Intro"), TEXT("Cam"), this))) return E_FAIL;
	}
	else if (m_pGameInstance->Get_Current_Level() == (_uint)LEVEL::LEVEL_BOSS2)
	{
		if (FAILED(m_pGameInstance->Add_Actor(TEXT("Ending"), TEXT("Cam"), this))) return E_FAIL;
	}
	

	return S_OK;
}

void CCameraPoint::Priority_Tick(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());
}

void CCameraPoint::Tick(_float fTimeDelta)
{
	
}

void CCameraPoint::Late_Tick(_float fTimeDelta)
{
	
	if (FAILED(m_pGameInstance->Add_DebugRender(m_pColliderCom)))
		return;
}

HRESULT CCameraPoint::Render()
{

	return S_OK;
}

void CCameraPoint::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	
}

void CCameraPoint::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void CCameraPoint::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CCameraPoint::Ready_Component()
{
	CBounding_AABB::BOUNDING_AABB_DESC AABB_Desc = {};
	AABB_Desc.pOnwer = this;
	AABB_Desc.eType = CBounding::TYPE::TYPE_AABB;
	AABB_Desc.bUseCol = true;
	AABB_Desc.vExtents = _float3(0.2f, 0.3f, 0.2f);
	AABB_Desc.vCenter = _float3(0.f, AABB_Desc.vExtents.y, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom,&AABB_Desc))) return E_FAIL;

	return S_OK;
}

CCameraPoint* CCameraPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCameraPoint* pInstance = new CCameraPoint(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCameraPoint");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCameraPoint::Clone(void* pArg)
{
	CCameraPoint* pInstance = new CCameraPoint(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCameraPoint");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCameraPoint::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}

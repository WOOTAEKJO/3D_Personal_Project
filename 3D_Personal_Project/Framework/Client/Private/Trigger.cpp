#include "stdafx.h"
#include "..\Public\Trigger.h"

#include "GameInstance.h"

CTrigger::CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CTrigger::CTrigger(const CTrigger& rhs)
	:CGameObject(rhs)
{
}

HRESULT CTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrigger::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	TRIGGER_DESC* pTriggerDesc = (TRIGGER_DESC*)pArg;

	m_strEventName = pTriggerDesc->strEventName;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_TRIGGER, m_pColliderCom)))
		return E_FAIL;

	_vector vPos = XMLoadFloat4(&pTriggerDesc->vPosition);
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	m_pTransformCom->Set_Scaling(pTriggerDesc->vScale.x, pTriggerDesc->vScale.y, pTriggerDesc->vScale.z);

	return S_OK;
}

void CTrigger::Priority_Tick(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());
}

void CTrigger::Tick(_float fTimeDelta)
{
	
}

void CTrigger::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CTrigger::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void CTrigger::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	
	if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_PLAYER && !m_bCheck)
	{
		m_pGameInstance->Execute_Event(m_strEventName);
		m_bCheck = true;
	}
		
}

void CTrigger::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void CTrigger::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CTrigger::Ready_Component()
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

CTrigger* CTrigger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrigger* pInstance = new CTrigger(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTrigger::Clone(void* pArg)
{
	CTrigger* pInstance = new CTrigger(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrigger::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);

}

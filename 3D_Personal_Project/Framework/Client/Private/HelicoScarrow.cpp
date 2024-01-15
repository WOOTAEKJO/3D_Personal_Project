#include "stdafx.h"
#include "..\Public\HelicoScarrow.h"

#include "Helico_IDLE.h"

CHelicoScarrow::CHelicoScarrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CHelicoScarrow::CHelicoScarrow(const CHelicoScarrow& rhs)
	:CMonster(rhs)
{
}

HRESULT CHelicoScarrow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHelicoScarrow::Initialize(void* pArg)
{

	if (FAILED(CMonster::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_MONSTER, m_pColliderCom)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Load_Data_Json(m_strModelTag, this)))
		return E_FAIL;*/

	m_Status_Desc.iMaxHP = 5;
	m_Status_Desc.iCurHP = 5;

	return S_OK;
}

void CHelicoScarrow::Priority_Tick(_float fTimeDelta)
{
	CMonster::Priority_Tick(fTimeDelta);
}

void CHelicoScarrow::Tick(_float fTimeDelta)
{
	CMonster::Tick(fTimeDelta);
}

void CHelicoScarrow::Late_Tick(_float fTimeDelta)
{

	CMonster::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	/*if (m_Status_Desc.iCurHP <= 0)
		Set_Dead();*/
}

HRESULT CHelicoScarrow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(CMonster::Render()))
		return E_FAIL;

	return S_OK;
}

void CHelicoScarrow::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	
}

void CHelicoScarrow::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
	
}

void CHelicoScarrow::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CHelicoScarrow::Bind_ShaderResources()
{
	if (FAILED(CMonster::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHelicoScarrow::Ready_Component()
{
	if (FAILED(CMonster::Ready_Component()))
		return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.bUseCol = true;
	Sphere_Desc.fRadius = 0.7f;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CHelicoScarrow::Ready_State()
{

	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CHelico_IDLE::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

CHelicoScarrow* CHelicoScarrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHelicoScarrow* pInstance = new CHelicoScarrow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHelicoScarrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHelicoScarrow::Clone(void* pArg)
{
	CHelicoScarrow* pInstance = new CHelicoScarrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHelicoScarrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelicoScarrow::Free()
{
	__super::Free();
	
}

#include "stdafx.h"
#include "..\Public\Spooketon.h"

#include "NorMonster_IDLE.h"
#include "NorMonster_Move.h"
#include "NorMonster_Attack.h"
#include "NorMonster_Delay.h"
#include "NorMonster_Dead.h"
#include "NorMonster_Hited.h"

#include "Bone.h"

CSpooketon::CSpooketon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CSpooketon::CSpooketon(const CSpooketon& rhs)
	:CMonster(rhs)
{
}

HRESULT CSpooketon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpooketon::Initialize(void* pArg)
{
	m_strModelTag = ANIMMODEL_SPOOKETON_TAG;

	CGameObject::GAMEOBJECT_DESC GameObject_Desc = {};

	GameObject_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	GameObject_Desc.fSpeedPerSec = 5.f;

	if (FAILED(CMonster::Initialize(&GameObject_Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(30.f, 0.f, 10.f, 1.f));
	m_pNavigationCom->Find_CurrentCell(m_pTransformCom->Get_State(CTransform::STATE::STATE_POS));

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDET_LAYER::COL_MONSTER, m_pColliderCom)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDET_LAYER::COL_MONSTER_BULLET, m_pWeaponColliderCom)))
		return E_FAIL;

	m_pSocketBone = m_pModelCom->Get_Bone(34);
	Safe_AddRef(m_pSocketBone);

	if (FAILED(m_pGameInstance->Load_Data_Json(m_strModelTag, this)))
		return E_FAIL;

	m_Status_Desc.iMaxHP = 5;
	m_Status_Desc.iCurHP = 5;

	return S_OK;
}

void CSpooketon::Priority_Tick(_float fTimeDelta)
{
	m_pWeaponColliderCom->Update(m_pSocketBone->Get_CombinedTransformationMatrix() * m_pTransformCom->Get_WorldMatrix_Matrix());
	CMonster::Priority_Tick(fTimeDelta);
}

void CSpooketon::Tick(_float fTimeDelta)
{
	CMonster::Tick(fTimeDelta);
}

void CSpooketon::Late_Tick(_float fTimeDelta)
{

	CMonster::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	if (m_Status_Desc.iCurHP <= 0)
		Set_Dead();
}

HRESULT CSpooketon::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(CMonster::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pWeaponColliderCom->Render();
#endif


	return S_OK;
}

void CSpooketon::Load_FromJson(const json& In_Json)
{
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Load_FromJson(In_Json);
}

void CSpooketon::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	if (iColID == m_pWeaponColliderCom->Get_Collider_ID())
	{
		m_pWeaponColliderCom->Set_UseCol(false);
	}

	if (iColID == m_pColliderCom->Get_Collider_ID())
	{
		if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDET_LAYER::COL_PLAYER_BULLET 
			&& !m_Status_Desc.bHited)
		{
			if (m_pStateMachineCom->Get_StateID() != (_uint)STATE::ATTACK)
			{
				m_Status_Desc.bHited = true;
			}
			m_Status_Desc.iCurHP -= 1.f;
		}
	}
}

void CSpooketon::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void CSpooketon::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CSpooketon::Bind_ShaderResources()
{
	if (FAILED(CMonster::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpooketon::Ready_Component()
{
	if (FAILED(CMonster::Ready_Component()))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC AABB_Desc = {};
	AABB_Desc.pOnwer = this;
	AABB_Desc.eType = CBounding::TYPE::TYPE_AABB;
	AABB_Desc.bUseCol = true;
	AABB_Desc.vExtents = _float3(0.5f, 1.f, 0.5f);
	AABB_Desc.vCenter = _float3(0.f, AABB_Desc.vExtents.y, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &AABB_Desc))) return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.fRadius = 0.6f;
	Sphere_Desc.vCenter = _float3(0.f, 0.1f, 0.3f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pWeaponColliderCom, &Sphere_Desc,1))) return E_FAIL;

	return S_OK;
}

HRESULT CSpooketon::Ready_State()
{
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CNorMonster_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::MOVE, CNorMonster_Move::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ATTACK, CNorMonster_Attack::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DEAD, CNorMonster_Dead::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DELAY, CNorMonster_Delay::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HITED, CNorMonster_Hited::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

CSpooketon* CSpooketon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpooketon* pInstance = new CSpooketon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpooketon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSpooketon::Clone(void* pArg)
{
	CSpooketon* pInstance = new CSpooketon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpooketon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpooketon::Free()
{
	__super::Free();
	
}

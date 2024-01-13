#include "stdafx.h"
#include "..\Public\SkullCrossBow.h"

#include "NorMonster_IDLE.h"
#include "NorMonster_Move.h"
#include "NorMonster_Attack.h"
#include "NorMonster_Delay.h"
#include "NorMonster_Dead.h"
#include "NorMonster_Hited.h"

#include "Bone.h"

#include "Normal_Bullet.h"

CSkullCrossBow::CSkullCrossBow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CSkullCrossBow::CSkullCrossBow(const CSkullCrossBow& rhs)
	:CMonster(rhs)
{
}

HRESULT CSkullCrossBow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkullCrossBow::Initialize(void* pArg)
{

	if (FAILED(CMonster::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_eMonsterType = CMonster::MONSTER_TYPE::SKULLCROSSBOW;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_MONSTER, m_pColliderCom)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_MONSTER_BULLET, m_pWeaponColliderCom)))
		return E_FAIL;*/

	/*m_pSocketBone = m_pModelCom->Get_Bone(34);
	Safe_AddRef(m_pSocketBone);*/

	if (FAILED(m_pGameInstance->Load_Data_Json(m_strModelTag, this)))
		return E_FAIL;

	m_Status_Desc.iMaxHP = 5;
	m_Status_Desc.iCurHP = 5;

	m_Status_Desc.fDetection_Range = 5.f;
	m_Status_Desc.fAttack_Range = 3.f;

	m_Physics_Desc.fForwardSpeed = 0.3f;

	m_bActivate = false;

	return S_OK;
}

void CSkullCrossBow::Priority_Tick(_float fTimeDelta)
{
	CMonster::Priority_Tick(fTimeDelta);
}

void CSkullCrossBow::Tick(_float fTimeDelta)
{
	CMonster::Tick(fTimeDelta);
}

void CSkullCrossBow::Late_Tick(_float fTimeDelta)
{

	CMonster::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	if (m_Status_Desc.iCurHP <= 0)
		Set_Dead();
}

HRESULT CSkullCrossBow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(CMonster::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkullCrossBow::Create_Bullet()
{
	CNormal_Bullet::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.05f;
	BulletDesc.fLifeTime = 2.f;
	BulletDesc.fSpeed = 3.f;
	BulletDesc.pTarget = m_pPlayer;

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS) +
		XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK)) * 0.1f;

	vPos.m128_f32[1] += m_pTransformCom->Get_Scaled().y;

	XMStoreFloat4(&BulletDesc.fStartPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_NORMAL_BULLET_TAG, &BulletDesc)))
		return E_FAIL;

	return S_OK;
}

void CSkullCrossBow::Load_FromJson(const json& In_Json)
{
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Load_FromJson(In_Json);
}

void CSkullCrossBow::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{

	if (iColID == m_pColliderCom->Get_Collider_ID())
	{
		if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_PLAYER_BULLET 
			&&!m_Status_Desc.bHited)
		{
			if (m_pStateMachineCom->Get_StateID() != (_uint)STATE::ATTACK)
			{
				m_Status_Desc.bHited = true;
			}
			m_Status_Desc.iCurHP -= 1;
		}
	}
}

void CSkullCrossBow::OnCollisionStay(CCollider* pCollider, _uint iColID)
{

}

void CSkullCrossBow::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CSkullCrossBow::Bind_ShaderResources()
{
	if (FAILED(CMonster::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkullCrossBow::Ready_Component()
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

	return S_OK;
}

HRESULT CSkullCrossBow::Ready_State()
{

	if (FAILED(__super::Ready_State()))
		return E_FAIL;

	return S_OK;
}

CSkullCrossBow* CSkullCrossBow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkullCrossBow* pInstance = new CSkullCrossBow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkullCrossBow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSkullCrossBow::Clone(void* pArg)
{
	CSkullCrossBow* pInstance = new CSkullCrossBow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkullCrossBow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkullCrossBow::Free()
{
	__super::Free();
	
}

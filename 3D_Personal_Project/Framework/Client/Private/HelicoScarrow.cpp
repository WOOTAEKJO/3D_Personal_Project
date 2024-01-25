#include "stdafx.h"
#include "..\Public\HelicoScarrow.h"

#include "Helico_Dead.h"
#include "Helico_Dive.h"
#include "Helico_Dive_IDLE.h"
#include "Helico_Exhausted_Hit.h"
#include "Helico_Exhausted_IDLE.h"
#include "Helico_Exhausted_Wake.h"
#include "Helico_Attack.h"
#include "Helico_Attack_End.h"
#include "Helico_Attack_Start.h"
#include "Helico_IDLE.h"
#include "Helico_Dive_Up.h"
#include "Helico_Ground_Smash.h"

#include "Bone.h"

#include "Helico_Bullet.h"
#include "Shock_Wave.h"



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

	m_pSocketBone = m_pModelCom->Get_Bone(2);
	Safe_AddRef(m_pSocketBone);

	/*m_Status_Desc.iMaxHP = 5;
	m_Status_Desc.iCurHP = 5;*/

	m_Status_Desc.bAttack_able = false;
	m_Status_Desc.bTalk = true;

	return S_OK;
}

void CHelicoScarrow::Priority_Tick(_float fTimeDelta)
{
	if (!m_bActivate)
		return;

	m_pColliderCom->Update(m_pSocketBone->Get_CombinedTransformationMatrix() * m_pTransformCom->Get_WorldMatrix_Matrix());
	CGameObject::Priority_Tick(fTimeDelta);
}

void CHelicoScarrow::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_0))
		m_Status_Desc.bTalk = false;

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

_bool CHelicoScarrow::Is_AllHited(_uint iNum)
{
	if (iNum >= 2)
		return false;

	if (m_iHited_Count[iNum] >= 3)
		return true;

	return false;
}

void CHelicoScarrow::Reset_Hited()
{
	m_iHited_Count[1] = 0;
}

void CHelicoScarrow::Create_Monster()
{
	CGameObject* pMonst = nullptr;
	CCharacter::CHARACTER_DESC Character_Desc = {};

	_float4 vPos[3] = { _float4(18.131f,2.f,19.256,1.f),_float4(18.001f,2.f,20.420f,1.f),
	_float4(19.168f,2.f,20.090f,1.f) };

	for (_uint i = 0; i < 3; i++)
	{
		Character_Desc.fRotationPerSec = XMConvertToRadians(90.f);
		Character_Desc.fSpeedPerSec = 5.f;
		Character_Desc.strModelTag = ANIMMODEL_SPOOKETON_TAG;

		if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_MONSTER],
			ANIMMODEL_SPOOKETON_TAG, &Character_Desc, reinterpret_cast<CGameObject**>(&pMonst))))
			return;

		m_vecSubMonster.push_back(pMonst);

		CTransform* pMonsterTrans = pMonst->Get_Component<CTransform>();

		pMonsterTrans->Set_Scaling(0.16f, 0.16f, 0.16f);
		pMonsterTrans->Set_State(CTransform::STATE::STATE_POS, vPos[i]);

		dynamic_cast<CMonster*>(pMonst)->Set_Activate();

		if (i == 2)
			pMonst->Get_Component<CNavigation>()->Set_CurrentIndex(65);
		else
			pMonst->Get_Component<CNavigation>()->Set_CurrentIndex(58);
	}
}

_bool CHelicoScarrow::Is_SubMonster_AllDead()
{
	for(auto& iter : m_vecSubMonster)
	{
		if (!iter->Get_Dead())
			return false;
	}

	m_vecSubMonster.clear();

	return true;
}

void CHelicoScarrow::Dead_CountDown()
{
	m_iHited_Count[0] += 1;
}

void CHelicoScarrow::Creat_Bullet()
{
	CHelico_Bullet::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.1f;
	BulletDesc.fLifeTime = 1.f;
	BulletDesc.fSpeed = 6.f;
	BulletDesc.pTarget = m_pPlayer;

	_uint iNum = 0;

	if (m_bSelectBonNum)
	{
		iNum = 6;
		m_bSelectBonNum = false;
	}
	else {
		iNum = 12;
		m_bSelectBonNum = true;
	}

	CBone* pBone = m_pModelCom->Get_Bone(iNum);

	_matrix matWorld = pBone->Get_CombinedTransformationMatrix() *
		m_pTransformCom->Get_WorldMatrix_Matrix();

	_vector vBonePos;

	memcpy(&vBonePos, &matWorld.r[3], sizeof(_float4));

	XMStoreFloat4(&BulletDesc.fStartPos, vBonePos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		MODEL_HELICOBULLET_TAG, &BulletDesc)))
		return;
		
}

void CHelicoScarrow::Create_Shock_Wave()
{
	CShock_Wave::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.3f;
	BulletDesc.fLifeTime = 0.9f;
	BulletDesc.fSpeed = 2.f;
	BulletDesc.pTarget = nullptr;
	
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMVector3Normalize( m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK));

	vPos = vPos + vLook * 0.1f;

	vPos.m128_f32[1] -= m_pTransformCom->Get_Scaled().y;

	XMStoreFloat4(&BulletDesc.fStartPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_SHOCK_WAVE_TAG, &BulletDesc)))
		return;
}

void CHelicoScarrow::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	if ((pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_PLAYER_BULLET) &&
		m_Status_Desc.bHited == false)
	{
		m_Status_Desc.bHited = true;

		m_iHited_Count[1] += 1;
	}
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
	Sphere_Desc.bUseCol = false;
	Sphere_Desc.fRadius = 0.7f;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CHelicoScarrow::Ready_State()
{

	if (FAILED(m_pStateMachineCom->Add_State(STATE::DEAD, CHelico_Dead::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DIVE, CHelico_Dive::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DIVE_IDLE, CHelico_Dive_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::EXHAUSTED_HIT, CHelico_Exhausted_Hit::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::EXHAUSTED_IDLE, CHelico_Exhausted_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::EXHAUSTED_WAKE, CHelico_Exhausted_Wake::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HELICO_ATTACK, CHelico_Attack::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HELICO_END, CHelico_Attack_End::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HELICO_START, CHelico_Attack_Start::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CHelico_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DIVE_UP, CHelico_Dive_Up::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::GROUND_SMASH, CHelico_Ground_Smash::Create(this)))) return E_FAIL;

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

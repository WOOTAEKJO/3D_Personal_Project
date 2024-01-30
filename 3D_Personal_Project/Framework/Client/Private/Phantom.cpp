#include "stdafx.h"
#include "..\Public\Phantom.h"

#include "Bone.h"

#include "Shock_Wave.h"
#include "Laser.h"
#include "Bomb.h"
#include "Target_Bullet.h"
#include "Meteor.h"
#include "Multiply.h"

#include "Phantom_Attaque_Chasse.h"
#include "Phantom_Chasse.h"
#include "Phantom_Hit_Chasse.h"
#include "Phantom_IDLE.h"
#include "Phantom_Appear.h"
#include "Phantom_Bouh.h"
#include "Phantom_Hit.h"
#include "Phantom_Intro.h"
#include "Phantom_Laser.h"
#include "Phantom_Marteau.h"
#include "Phantom_Dead.h"
#include "Phantom_Shoot.h"
#include "Phantom_Summon.h"
#include "Phantom_Summon_Bomb.h"
#include "Phantom_Summon_Loop.h"
#include "Phantom_Vanish.h"
#include "Phantom_Dash.h"

#include "Plateform.h"
#include "Cell.h"

#include "Utility_Effect.h"
#include "Effect_Reaper.h"
#include "Effect_Laser.h"
#include "Effect_DashRoad.h"
#include "Effect_Energy.h"
#include "Effect_Target.h"

CPhantom::CPhantom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CPhantom::CPhantom(const CPhantom& rhs)
	:CMonster(rhs)
{
}

HRESULT CPhantom::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhantom::Initialize(void* pArg)
{

	if (FAILED(CMonster::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_MONSTER, m_pColliderCom)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Data_Json(m_strModelTag, this)))
		return E_FAIL;

	if (FAILED(Init_Point_Light()))
		return E_FAIL;

	m_pSocketBone = m_pModelCom->Get_Bone(5);
	Safe_AddRef(m_pSocketBone);

	/*m_Status_Desc.iMaxHP = 5;
	m_Status_Desc.iCurHP = 5;*/

	m_pTransformCom->Set_Scaling(0.5f, 0.5f, 0.5f);

	m_Status_Desc.bAttack_able = false;
	m_Status_Desc.bTalk = true;

	SetUp_Random_Pos();

	m_eCurrentPhase = PHASE::PAHSE1;
	/*m_eCurrentPhase = PHASE::PAHSE2;
	m_iHitCount = 2;*/
	
	CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_BOSS2IDLE_TAG, GO_PARTICLEALWAYS_TAG,
		this, &m_pIDLEParicle);

	return S_OK;
}

void CPhantom::Priority_Tick(_float fTimeDelta)
{
	if (!m_bActivate)
		return;

	

	m_pColliderCom->Update(m_pSocketBone->Get_CombinedTransformationMatrix() * m_pTransformCom->Get_WorldMatrix_Matrix());
	CGameObject::Priority_Tick(fTimeDelta);
}

void CPhantom::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_0))
		m_Status_Desc.bTalk = false;

	if (m_Status_Desc.bTalk)
		XMStoreFloat4(&m_vOriginPos, m_pTransformCom->Get_State(CTransform::STATE::STATE_POS));

	CMonster::Tick(fTimeDelta);
}

void CPhantom::Late_Tick(_float fTimeDelta)
{
	Shock_Wave_Radius_Compute();

	CMonster::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	Judge_Dead();

}

HRESULT CPhantom::Render()
{
	if (!m_bActivate)
		return S_OK;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

_uint CPhantom::Get_PrevState()
{
	return m_pStateMachineCom->Get_PrevID();
}

void CPhantom::Load_FromJson(const json& In_Json)
{
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Load_FromJson(In_Json);
}


void CPhantom::Create_Shock_Wave()
{
	CShock_Wave::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.3f;
	BulletDesc.fLifeTime = 1.5f;
	BulletDesc.fSpeed = 2.2f;
	BulletDesc.pTarget = nullptr;
	
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMVector3Normalize( m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK));

	_vector vWavePos = vPos + vLook * 2.f;

	vWavePos.m128_f32[1] -= m_pTransformCom->Get_Scaled().y * 1.f;

	XMStoreFloat4(&BulletDesc.fStartPos, vWavePos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_SHOCK_WAVE_TAG, &BulletDesc,&m_pShockWave_Col)))
		return;

	//Safe_AddRef(m_pShockWave_Col);

	vWavePos.m128_f32[1] += 0.3f;

	_float4 vParticlePos;
	XMStoreFloat4(&vParticlePos, vWavePos);

	CUtility_Effect::Create_Particle_Attack(m_pGameInstance, PARTICLE_BOSS2WAVE_TAG, GO_PARTICLEATTACK_TAG,
		this, vParticlePos, _float3(0.f, 0.f, 0.f), nullptr, 1.5f);

	CUtility_Effect::Create_Effect_Reaper(m_pGameInstance, this, vWavePos, _float4(0.f, 0.8f, 1.f, 1.f),
		&m_pShockWave_Effect, 1.5f, _float2(8.f, 8.f));

	//Safe_AddRef(m_pShockWave_Effect);

	CEffect::EFFECTINFO Info = {};
	Info.pOwner = this;
	Info.fLifeTime = 0.5f;
	Info.strEffectTextureTag = TEX_DAMAGEIMPACT_TAG;
	Info.vSize = _float2(1.5f, 1.5f);
	Info.vPos = vParticlePos;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTDAMAGE_TAG, &Info)))
		return;
}

void CPhantom::Shock_Wave_Radius_Compute()
{
	if (!m_bSmashTime)
		return;

	if (m_pShockWave_Col == nullptr || m_pShockWave_Col->Get_Dead())
		return;

	if (m_pShockWave_Effect == nullptr || m_pShockWave_Effect->Get_Dead())
		return;

	_vector vColPos = dynamic_cast<CShock_Wave*>(m_pShockWave_Col)->Get_ColWorldMat(1).r[3];
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK);

	vPos += XMVector3Normalize(vLook) * 2.f;

	_vector vDir = vPos - vColPos;

	_float fDistance = XMVectorGetX(XMVector3Length(vDir));

	dynamic_cast<CEffect_Reaper*>(m_pShockWave_Effect)->Set_Radius(fDistance);
}

void CPhantom::Create_Laser()
{
	CShock_Wave::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.3f;
	BulletDesc.fLifeTime = 0.f;
	BulletDesc.fSpeed = 0.f;
	BulletDesc.pTarget = m_pPlayer;

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK));

	vPos = vPos;// - vLook * 0.3f;

	vPos.m128_f32[1] += m_pTransformCom->Get_Scaled().y * 12.f;

	XMStoreFloat4(&BulletDesc.fStartPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_LASER_TAG, &BulletDesc, reinterpret_cast<CGameObject**>(&m_pLaser))))
		return;

	CEffect_Laser::EFFECT_LASERINFO Info = {};
	Info.pOwner = m_pLaser;
	Info.fLifeTime = 0.f;
	Info.strEffectTextureTag = TEX_LASER_TAG;
	Info.vSize = _float2(1.f, 1.f);
	Info.vColor = _float4(0.f, 1.f, 1.f, 1.f);
	Info.pTarger = m_pPlayer;

	XMStoreFloat4(&Info.vPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTLASER_TAG, &Info)))
		return;
}

void CPhantom::Update_Laser_Elec()
{
}

void CPhantom::Adjust_Pos(_float3 vAdjust)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos = vPos + XMLoadFloat3(&vAdjust);

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
}

void CPhantom::Delete_Laser()
{
	if (m_pLaser == nullptr || m_pLaser->Get_Dead())
		return;

	dynamic_cast<CLaser*>(m_pLaser)->Set_Dead();
	m_pLaser = nullptr;
}

void CPhantom::Create_Multiply()
{

	_uint iNum = 0;

	if (0 <= m_iPrevRandomNum && m_iPrevRandomNum < 5)
	{
		iNum = Random({ 5,6,7,8,9,10,11,12,13,14,15,16,17,18,19});
	}
	else if (5 <= m_iPrevRandomNum && m_iPrevRandomNum < 10)
	{
		iNum = Random({ 0,1,2,3,4,10,11,12,13,14,15,16,17,18,19 });
	}
	else if (10 <= m_iPrevRandomNum && m_iPrevRandomNum < 15)
	{
		iNum = Random({ 0,1,2,3,4,5,6,7,8,9,15,16,17,18,19 });
	}
	else if (15 <= m_iPrevRandomNum && m_iPrevRandomNum < 20)
	{
		iNum = Random({ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
	}

	m_iPrevRandomNum = iNum;

	CMultiply::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.3f;
	BulletDesc.fLifeTime = 1.4f;
	BulletDesc.fSpeed = 15.f;
	BulletDesc.pTarget = m_pPlayer;
	BulletDesc.fStartPos = _float4(m_vRandomPos[iNum].x, m_vRandomPos[iNum].y, m_vRandomPos[iNum].z,1.f);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_MULTIPLY_TAG, &BulletDesc)))
		return;
}

void CPhantom::SetUp_Random_Pos()
{

	_float2 fStartXZ = {};
	_float fY = 5.5f;
	_float fSign = 1.f;

	/* 0 ~ 4*/
	fStartXZ = _float2(11.6f, 17.6f);
	for (_uint i = 0; i < 5; i++)
	{
		m_vRandomPos[i].x = i * fSign + fStartXZ.x -1;
		m_vRandomPos[i].y = fY;
		m_vRandomPos[i].z = i * -fSign + fStartXZ.y -1;
	}

	/* 5 ~ 9*/
	fStartXZ = _float2(17.6f, 13.6f);
	for (_uint i = 0; i < 5; i++)
	{
		m_vRandomPos[i + 5].x = i * fSign + fStartXZ.x + 1;
		m_vRandomPos[i + 5].y = fY;
		m_vRandomPos[i + 5].z = i * fSign + fStartXZ.y - 1;
	}

	/* 10 ~ 14*/
	fStartXZ = _float2(21.6f, 19.6f);
	for (_uint i = 0; i < 5; i++)
	{
		m_vRandomPos[i + 10].x = i * -fSign + fStartXZ.x + 1;
		m_vRandomPos[i + 10].y = fY;
		m_vRandomPos[i + 10].z = i * fSign + fStartXZ.y + 1;
	}

	/* 15 ~ 19*/
	fStartXZ = _float2(15.6f, 23.6f);
	for (_uint i = 0; i < 5; i++)
	{
		m_vRandomPos[i + 15].x = i * -fSign + fStartXZ.x -1;
		m_vRandomPos[i + 15].y = fY;
		m_vRandomPos[i + 15].z = i * -fSign + fStartXZ.y + 1;
	}

	/*for (_uint i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			fStartXZ = _float2(11.6f, 17.6f);
		}
		else if (i == 1)
		{
			fStartXZ = _float2(17.6f, 13.6);
		}
		else if (i == 2)
		{
			fStartXZ = _float2(11.6f, 19.6);
		}
		else if (i == 3)
		{
			fStartXZ = _float2(17.6, 23.6);
		}

		for (_uint j = 0; j < 5; j++)
		{

		}
	}*/
	
}

void CPhantom::Appear_OriginPos()
{
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMLoadFloat4(&m_vOriginPos));
	TargetLook();
}

void CPhantom::SetUp_OriginPos(_float4 vPos)
{
	m_vOriginPos = vPos;
}

void CPhantom::Modify_Pos(_float3 vPos)
{
	_float4 vTrans = _float4(m_vOriginPos.x + vPos.x, m_vOriginPos.y + vPos.y,
		m_vOriginPos.z + vPos.z, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMLoadFloat4(&vTrans));
	TargetLook();
}

void CPhantom::Appear_PlayerPos()
{
	_vector vPlayerPos = m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS);
	_vector vPlayerLook = m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_LOOK);

	vPlayerPos += XMVector3Normalize(vPlayerLook) * 2.f;
	vPlayerPos.m128_f32[1] = m_vOriginPos.y;
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPlayerPos);
	TargetLook();
}

void CPhantom::Create_Bomb()
{
	CBomb::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_TRIGGER_BULLET;
	BulletDesc.fRadius = 0.3f;
	BulletDesc.fLifeTime = 0.f;
	BulletDesc.fSpeed =5.f;
	BulletDesc.pTarget = m_pPlayer;

	for (_uint i = 0; i < 3; i++)
	{
		BulletDesc.fStartPos = _float4(10.2f + ((i == 2 ? 3.f : 1.5f) * i),5.f,17.2f - ((i == 2 ? 3.f : 1.5f) * i),1.f);

		if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
			GO_BOMB_TAG, &BulletDesc, reinterpret_cast<CGameObject**>(&m_pBomb[i]))))
			return;

		_float4 vTargetPos = _float4(13.6f + ((i == 2 ? 1.5f : 1.f) * i), 7.f,18.6f - 
			((i == 2 ? 1.4f : 1.f) * i),1.f);

		dynamic_cast<CBomb*>(m_pBomb[i])->Set_TartgetPos(vTargetPos);
	}
}

void CPhantom::Start_Point_Toward_Bomb()
{
	for (_uint i = 0; i < 3; i++)
	{
		if (m_pBomb[i] != nullptr || !m_pBomb[i]->Get_Dead())
		{
			dynamic_cast<CBomb*>(m_pBomb[i])->Set_Start(true);
		}
	}
}

void CPhantom::Delete_Failed_Bomb()
{
	for (_uint i = 0; i < 3; i++)
	{
		if (m_pBomb[i] != nullptr || !m_pBomb[i]->Get_Dead())
		{
			dynamic_cast<CBomb*>(m_pBomb[i])->Set_Dead();
		}
	}
}

void CPhantom::Delete_Bomb()
{
	for (_uint i = 0; i < 3; i++)
	{
		if (m_pBomb[i] != nullptr || !m_pBomb[i]->Get_Dead())
		{
			dynamic_cast<CBomb*>(m_pBomb[i])->Dead_Particle();
		}
	}
}

_bool CPhantom::Is_Bomb_Failed()
{
	for (_uint i = 0; i < 3; i++)
	{
		if (m_pBomb[i] != nullptr || !m_pBomb[i]->Get_Dead()) {
			if (!dynamic_cast<CBomb*>(m_pBomb[i])->Is_Failed())
			{
				return false;
			}
		}
		else {
			return false;
		}
	}

	return true;
}

void CPhantom::Create_TargetBullet()
{
	CGameObject* pBullet = nullptr;

	CTarget_Bullet::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.2f;
	BulletDesc.fLifeTime = 0.9f;
	BulletDesc.fSpeed = 10.f;
	BulletDesc.pTarget = m_pPlayer;

	// 손 뼈 정보 찾아서 위치 주기

	CBone* pBone = m_pModelCom->Get_Bone(24);

	_matrix matWorld = pBone->Get_CombinedTransformationMatrix() *
		m_pTransformCom->Get_WorldMatrix_Matrix();

	_vector vBonePos;

	memcpy(&vBonePos, &matWorld.r[3], sizeof(_float4));

	XMStoreFloat4(&BulletDesc.fStartPos, vBonePos);
	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_TARGETBULLET_TAG, &BulletDesc, reinterpret_cast<CGameObject**>(&pBullet))))
		return;

	CEffect_Energy::EFFECT_ENERGYINFO Info = {};
	Info.pOwner = pBullet;
	Info.fLifeTime = 0.f;
	Info.strEffectTextureTag = TEX_BUBLE_TAG;
	Info.vSize = _float2(0.4f, 0.4f);
	Info.vColor = _float4(0.f, 1.f, 1.f, 1.f);
	Info.fSizeSpeed = -1.f;

	XMStoreFloat4(&Info.vPos, pBullet->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS));

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTENERGY_TAG, &Info)))
		return;
}

void CPhantom::Add_Hit_Count()
{
	m_iHitCount += 1;
}

_bool CPhantom::Judge_Hit()
{
	if (m_iHitCount >= 2 && m_eCurrentPhase == PHASE::PAHSE1)
		m_eCurrentPhase = PHASE::PAHSE2;
	else if (m_iHitCount >= 4 && m_eCurrentPhase == PHASE::PAHSE2)
		return true;

	return false;
}

void CPhantom::Create_Meteor()
{
	CMeteor::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.2f;
	BulletDesc.fLifeTime = 0.f;
	BulletDesc.fSpeed = 5.f;
	BulletDesc.pTarget = m_pPlayer;

	_float fX = Random({ -1.f,-0.7f,-0.5f,-0.3f,-0.2f,-0.1f,0.f,0.1f,0.2f,0.3f,0.5f,0.7f,1.f});
	_float fZ = Random({ -1.f,-0.7f,-0.5f,-0.3f,-0.2f,-0.1f,0.f,0.1f,0.2f,0.3f,0.5f,0.7f,1.f });

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS));
	vPos.x += fX;
	vPos.y = 15.f;
	vPos.z += fZ;
	vPos.w = 1.f;

	BulletDesc.fStartPos = vPos;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_METEOR_TAG, &BulletDesc)))
		return;


}

void CPhantom::Drop_Floor(_uint iFloorType)
{
	if (iFloorType >= 2)
		return;

	list<CGameObject*> Plateform = m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER::LAYER_PLATEFORM]);

	for (auto& iter : Plateform)
	{
		wstring strTag = CUtility_String::Get_LastName(dynamic_cast<CPlateform*>(iter)->Get_ModelTag());

		if (!wcscmp(strTag.c_str(),
			CUtility_String::Get_LastName(MODEL_TREE_PLATEFORME01X01_TAG).c_str()))
		{
			if (iFloorType == 0)
			{
				if (dynamic_cast<CPlateform*>(iter)->Get_TriggerNum() == -3)
				{
					dynamic_cast<CPlateform*>(iter)->Set_Fall();
				}
			}
			else if(iFloorType == 1) {
				if (dynamic_cast<CPlateform*>(iter)->Get_TriggerNum() == -4)
				{
					dynamic_cast<CPlateform*>(iter)->Set_Fall();
				}
			}
		}
	}
}

void CPhantom::Navi_Filter()
{
	vector<CCell*> pNaviCells = m_pNavigationCom->Get_Navigation_Cells();

	_uint iSize = pNaviCells.size();

	for (_uint i = 0; i < iSize; i++)
	{
		_int iAloneIndx = -1;

		if (m_pNavigationCom->Is_Alone_Neighbor_Cell(i, &iAloneIndx))
		{
			if (iAloneIndx != -1)
			{
				m_pNavigationCom->Set_Cell_Type(CCell::CELLTYPE::TYPE_UNABLE, i);
				m_pNavigationCom->Set_Cell_Type(CCell::CELLTYPE::TYPE_UNABLE, iAloneIndx);
				/*m_pNavigationCom->Delete_Cell(i);
				m_pNavigationCom->Delete_Cell(iAloneIndx);*/
			}
		}
	}

	m_pNavigationCom->Init_Neighbor_XZ();
}

_bool CPhantom::Is_Target_Near()
{
	_vector vPlayerPos = m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	if (XMVector3NearEqual(vPlayerPos, vPos, XMVectorSet(0.1f, 1000.f, 0.1f, 0.f)))
		return true;

	return false;
}

void CPhantom::Judge_Dead()
{
	if (m_pStateMachineCom->Get_StateID() == (_uint)CPhantom::STATE::DEAD)
	{
		if (m_pIDLEParicle != nullptr && !m_pIDLEParicle->Get_Dead())
		{
			m_pIDLEParicle->Set_Dead();
		}
	}
}

void CPhantom::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
	if (pCollider->Get_ColLayer_Type() == (_uint)COLLIDER_LAYER::COL_TRIGGER_BULLET)
	{
		Delete_Bomb();
	}
}

void CPhantom::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
	
}

void CPhantom::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CPhantom::Bind_ShaderResources()
{
	if (FAILED(CMonster::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPhantom::Ready_Component()
{
	/*if (FAILED(CMonster::Ready_Component()))
		return E_FAIL;*/

	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = 0;
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom, &NavigationDesc))) return E_FAIL;

	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;
	if (FAILED(Add_Component<CStateMachine>(COM_STATEMACHINE_TAG, &m_pStateMachineCom))) return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.bUseCol = false;
	Sphere_Desc.fRadius = 4.f;
	Sphere_Desc.vCenter = _float3(0.f, Sphere_Desc.fRadius - 2.f, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CPhantom::Ready_State()
{

	if (FAILED(m_pStateMachineCom->Add_State(STATE::ATTAQUE_CHASSE,CPhantom_Attaque_Chasse::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::CHASSE, CPhantom_Chasse::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HIT_CHASSE, CPhantom_Hit_Chasse::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CPhantom_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::APPEAR, CPhantom_Appear::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::BOUH, CPhantom_Bouh::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HIT, CPhantom_Hit::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::INTRO, CPhantom_Intro::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::LASER, CPhantom_Laser::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::MARTEAU, CPhantom_Marteau::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DEAD, CPhantom_Dead::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::SHOOT, CPhantom_Shoot::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::SUMMON, CPhantom_Summon::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::SUMMON_BOMB, CPhantom_Summon_Bomb::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::SUMMON_LOOP, CPhantom_Summon_Loop::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::VANISH, CPhantom_Vanish::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DASH, CPhantom_Dash::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPhantom::Init_Point_Light()
{
	LIGHT_DESC LightDesc = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] += m_pTransformCom->Get_Scaled().y * 3.f;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 2.f;
	LightDesc.vDiffuse = _float4(0.f, 0.7f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.f, 0.7f, 1.f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

	return S_OK;
}

CPhantom* CPhantom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPhantom* pInstance = new CPhantom(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPhantom");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPhantom::Clone(void* pArg)
{
	CPhantom* pInstance = new CPhantom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPhantom");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhantom::Free()
{
	__super::Free();

	/*Safe_Release(m_pShockWave_Col);
	Safe_Release(m_pShockWave_Effect);*/
}

#include "stdafx.h"
#include "..\Public\Phantom.h"

#include "Bone.h"

#include "Shock_Wave.h"
#include "Laser.h"
#include "Bomb.h"

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

	m_pSocketBone = m_pModelCom->Get_Bone(5);
	Safe_AddRef(m_pSocketBone);

	/*m_Status_Desc.iMaxHP = 5;
	m_Status_Desc.iCurHP = 5;*/

	m_pTransformCom->Set_Scaling(0.5f, 0.5f, 0.5f);

	m_Status_Desc.bAttack_able = false;
	m_Status_Desc.bTalk = true;

	SetUp_Random_Pos();

	m_eCurrentPhase = PHASE::PAHSE1;

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

	CMonster::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

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
	BulletDesc.fLifeTime = 0.9f;
	BulletDesc.fSpeed = 2.f;
	BulletDesc.pTarget = nullptr;
	
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMVector3Normalize( m_pTransformCom->Get_State(CTransform::STATE::STATE_LOOK));

	vPos = vPos + vLook * 2.f;

	vPos.m128_f32[1] -= m_pTransformCom->Get_Scaled().y * 1.f;

	XMStoreFloat4(&BulletDesc.fStartPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_BULLET],
		GO_SHOCK_WAVE_TAG, &BulletDesc)))
		return;
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
	m_pLaser == nullptr;
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

	CShock_Wave::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_MONSTER_BULLET;
	BulletDesc.fRadius = 0.3f;
	BulletDesc.fLifeTime = 0.9f;
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
	fStartXZ = _float2(17.6f, 13.6);
	for (_uint i = 0; i < 5; i++)
	{
		m_vRandomPos[i + 5].x = i * fSign + fStartXZ.x + 1;
		m_vRandomPos[i + 5].y = fY;
		m_vRandomPos[i + 5].z = i * fSign + fStartXZ.y - 1;
	}

	/* 10 ~ 14*/
	fStartXZ = _float2(21.6f, 19.6);
	for (_uint i = 0; i < 5; i++)
	{
		m_vRandomPos[i + 10].x = i * -fSign + fStartXZ.x + 1;
		m_vRandomPos[i + 10].y = fY;
		m_vRandomPos[i + 10].z = i * fSign + fStartXZ.y + 1;
	}

	/* 15 ~ 19*/
	fStartXZ = _float2(15.6, 23.6);
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
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPlayerPos);
	TargetLook();
}

void CPhantom::Create_Bomb()
{
	CBomb::BULLET_DESC BulletDesc = {};
	BulletDesc.pOwner = this;
	BulletDesc.eCollider_Layer = COLLIDER_LAYER::COL_PLAYER_BULLET;
	BulletDesc.fRadius = 0.3f;
	BulletDesc.fLifeTime = 0.f;
	BulletDesc.fSpeed =5.f;
	BulletDesc.pTarget = nullptr;

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
		if (m_pBomb[i] != nullptr)
		{
			dynamic_cast<CBomb*>(m_pBomb[i])->Set_Start(true);
		}
	}
}

void CPhantom::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{

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
}

#include "stdafx.h"
#include "Crow.h"

#include "GameInstance.h"

#include "Bone.h"
#include "Player.h"

#include "Monster.h"

CCrow::CCrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CNPC(pDevice, pContext)
{
}

CCrow::CCrow(const CCrow& rhs)
	: CNPC(rhs)
{
}

HRESULT CCrow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCrow::Initialize(void* pArg)
{
	CCrow::CHARACTER_DESC Crow_Desc = {};
	Crow_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Crow_Desc.fSpeedPerSec = 5.f;
	Crow_Desc.strModelTag = ANIMMODEL_CROW_TAG;

	if (FAILED(CNPC::Initialize(&Crow_Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_eNPCType = CNPC::NPC_TYPE::CROW;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_PLAYER_BULLET, m_pColliderCom);

	m_pTransformCom->Set_Scaling(0.16f, 0.16f, 0.16f);

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS,
		m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS));

	m_pTransformCom->Set_Ground(false);

	//m_Status_Desc.fDetection_Range = ;

	return S_OK;
}

void CCrow::Priority_Tick(_float fTimeDelta)
{
	//m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());

	Player_Head_Pos();
	Attack_Cool(3.f,fTimeDelta);

	CNPC::Priority_Tick(fTimeDelta);
}

void CCrow::Tick(_float fTimeDelta)
{
	CNPC::Tick(fTimeDelta);
}

void CCrow::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	if (FAILED(m_pGameInstance->Add_DebugRender(m_pColliderCom)))
		return;

	CNPC::Late_Tick(fTimeDelta);
	
}

HRESULT CCrow::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}


	return S_OK;
}

void CCrow::Set_IDLE_Pos()
{
	if (m_pPlayer_Transform == nullptr)
		return;

	

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, m_vTargetPos);
}

void CCrow::Player_Head_Pos()
{
	if (m_pPlayer_Transform == nullptr)
		return;

	if (m_eCurrentTypeAnimIndex == (_int)CNPC::STATE::ATTACK)
		return;

	_vector vPos = m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS);
	_float3 vScale = m_pPlayer_Transform->Get_Scaled();

	vPos.m128_f32[1] += vScale.y * 2.1f;

	XMStoreFloat4(&m_vTargetPos, vPos);
}

void CCrow::Attack_Cool(_float fCool,_float fTimeDelta)
{
	m_fAttackTime += fTimeDelta;

	if (m_fAttackTime > fCool)
	{
		m_fAttackTime = 0.f;
		m_bAttack = true;
	}
}

_bool CCrow::Attack_Input()
{
	if (m_bAttack && m_pGameInstance->Mouse_Down(DIM_RB))
	{
		m_bAttack = false;

		return true;
	}

	return false;
}

_bool CCrow::Find_Range_Monster(_float fRange)
{
	_bool bCheck = false;

	list<CGameObject*> listMonster = m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level()
		, g_strLayerName[LAYER::LAYER_MONSTER]);

	_float fMinDist = fRange;

	for (auto& iter : listMonster)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(iter);

		if (!pMonster->Is_Activate() || !pMonster->Open_Status_Desc()->bAttack_able)
			continue;

		CTransform* pMonsterTransform = iter->Get_Component<CTransform>();

		_vector vMonsterPos = pMonsterTransform->Get_State(CTransform::STATE::STATE_POS);
		_vector vPlayerPos = m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS);

		_vector vDir = (vMonsterPos - vPlayerPos);
		_vector vLook = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_LOOK);

		_float fDist = XMVectorGetX(XMVector3Length(vDir));

		if (fMinDist > fDist)
		{
			bCheck = true;

			fMinDist = fDist;

			//vMonsterPos.m128_f32[1] += pMonsterTransform->Get_Scaled().y * 0.5f;
			//XMStoreFloat4(&m_vTargetPos, vMonsterPos);

			_float4x4 matColWorld = pMonster->Get_Col_WorldMat();

			// 공격 위치를 몬스터의 월드 위치에서 몬스터가 가지고 있는 콜라이더의 월드 위치로 변경
			m_vTargetPos = _float4(matColWorld.m[3][0], matColWorld.m[3][1], matColWorld.m[3][2], 1.f);
		}
	}

	return bCheck;
}

_bool CCrow::Is_Col()
{
	return m_pColliderCom->Get_Collision();
}

void CCrow::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{

}

void CCrow::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void CCrow::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT CCrow::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix_Float4x4() )))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	_uint2 iIndx;
	if (m_eCurrentTypeAnimIndex == (_int)CNPC::STATE::FOLLOW || m_eCurrentTypeAnimIndex == (_int)CNPC::STATE::ATTACK)
		iIndx = { 1000,1000 };
	else
		iIndx = { 6,9 };
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_iDiscardIndx", &iIndx, sizeof(_uint2))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrow::Ready_Component()
{
	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = m_pPlayer->Get_Component<CNavigation>()->Get_CurrentIndex();
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom, &NavigationDesc))) return E_FAIL;

	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;
	if (FAILED(Add_Component<CStateMachine>(COM_STATEMACHINE_TAG, &m_pStateMachineCom))) return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.fRadius = 0.2f;
	Sphere_Desc.vCenter = _float3(0.f, 0.1f, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CCrow::Ready_State()
{
	if (FAILED(__super::Ready_State()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrow::Ready_Animation()
{
	Add_TypeAnimIndex(STATE::IDLE, 0);
	Add_TypeAnimIndex(STATE::FOLLOW, 1);
	Add_TypeAnimIndex(STATE::ATTACK, 1);
	Add_TypeAnimIndex(STATE::TALK, 2);

	return S_OK;
}

CCrow* CCrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCrow* pInstance = new CCrow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCrow::Clone(void* pArg)
{
	CCrow* pInstance = new CCrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrow::Free()
{
	__super::Free();
}

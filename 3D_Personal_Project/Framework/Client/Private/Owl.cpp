#include "stdafx.h"
#include "Owl.h"

#include "GameInstance.h"

#include "Bone.h"
#include "Player.h"

#include "Monster.h"
#include "Trigger.h"

COwl::COwl(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CNPC(pDevice, pContext)
{
}

COwl::COwl(const COwl& rhs)
	: CNPC(rhs)
{
}

HRESULT COwl::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COwl::Initialize(void* pArg)
{
	COwl::CHARACTER_DESC Owl_Desc = {};
	Owl_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Owl_Desc.fSpeedPerSec = 5.f;
	Owl_Desc.strModelTag = ANIMMODEL_OWL_TAG;

	if (FAILED(CNPC::Initialize(&Owl_Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_eNPCType = CNPC::NPC_TYPE::OWL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Init_Point_Light()))
		return E_FAIL;


	//m_pTransformCom->Set_Scaling(0.2f, 0.2f, 0.2f);

	m_Status_Desc.bHited = false;

	if (FAILED(m_pGameInstance->Add_Actor(TEXT("OwlTalk"), TEXT("Owl"), this)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_Event(TEXT("OwlTalk"), [this]() {

		m_pStateMachineCom->Set_State(COwl::STATE::TALK);

		})))
		return E_FAIL;


		CTrigger::TRIGGER_DESC TriggerDesc = {};
		TriggerDesc.strEventName = TEXT("OwlTalk");
		TriggerDesc.vPosition = _float4(6.f, 7.f, 3.f, 1.f);
		TriggerDesc.vScale = _float3(1.f, 1.f, 1.f);

		if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_PLATEFORM]
			, GO_TRIGGER_TAG, &TriggerDesc)))
			return E_FAIL;*/

	return S_OK;
}

void COwl::Priority_Tick(_float fTimeDelta)
{
	CNPC::Priority_Tick(fTimeDelta);
}

void COwl::Tick(_float fTimeDelta)
{
	CNPC::Tick(fTimeDelta);
}

void COwl::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	CNPC::Late_Tick(fTimeDelta);
	
}

HRESULT COwl::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void COwl::OnCollisionEnter(CCollider* pCollider, _uint iColID)
{
}

void COwl::OnCollisionStay(CCollider* pCollider, _uint iColID)
{
}

void COwl::OnCollisionExit(CCollider* pCollider, _uint iColID)
{
}

HRESULT COwl::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix_Float4x4() )))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bHited", &m_bHit_Effect, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}

HRESULT COwl::Ready_Component()
{
	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = m_pPlayer->Get_Component<CNavigation>()->Get_CurrentIndex();
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom, &NavigationDesc))) return E_FAIL;

	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;
	if (FAILED(Add_Component<CStateMachine>(COM_STATEMACHINE_TAG, &m_pStateMachineCom))) return E_FAIL;

	/*CBounding_Sphere::BOUNDING_SPHERE_DESC Sphere_Desc = {};
	Sphere_Desc.pOnwer = this;
	Sphere_Desc.eType = CBounding::TYPE::TYPE_SPHERE;
	Sphere_Desc.fRadius = 0.2f;
	Sphere_Desc.vCenter = _float3(0.f, 0.1f, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &Sphere_Desc))) return E_FAIL;*/
	return S_OK;
}

HRESULT COwl::Ready_State()
{

	if (FAILED(__super::Ready_State()))
		return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT COwl::Ready_Animation()
{
	Add_TypeAnimIndex(STATE::IDLE, 0);
	/*Add_TypeAnimIndex(STATE::FOLLOW, 1);
	Add_TypeAnimIndex(STATE::ATTACK, 1);*/
	Add_TypeAnimIndex(STATE::TALK, 2);

	return S_OK;
}

HRESULT COwl::Init_Point_Light()
{
	LIGHT_DESC LightDesc = {};
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] = m_pTransformCom->Get_Scaled().y;

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPos, vPos);
	LightDesc.fRange = 0.5f;
	LightDesc.vDiffuse = _float4(0.2f, 0.6f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.6f, 0.f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, reinterpret_cast<CLight**>(&m_pLight))))
		return E_FAIL;

	Safe_AddRef(m_pLight);

	return S_OK;
}

COwl* COwl::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	COwl* pInstance = new COwl(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COwl");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* COwl::Clone(void* pArg)
{
	COwl* pInstance = new COwl(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : COwl");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void COwl::Free()
{
	__super::Free();
}

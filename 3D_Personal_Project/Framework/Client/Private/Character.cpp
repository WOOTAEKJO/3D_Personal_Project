#include "stdafx.h"
#include "..\Public\Character.h"

#include "TargetCamera.h"

#include "GameInstance.h"
#include "Effect.h"

#include "Light.h"

CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& rhs)
	:CGameObject(rhs)
{
}

HRESULT CCharacter::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		CHARACTER_DESC* Character_Desc = ((CHARACTER_DESC*)pArg);
		m_strModelTag = Character_Desc->strModelTag;
	}

	return S_OK;
}

void CCharacter::Priority_Tick(_float fTimeDelta)
{
	if(m_pColliderCom != nullptr)
		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());

	CGameObject::Priority_Tick(fTimeDelta);
}

void CCharacter::Tick(_float fTimeDelta)
{
	CGameObject::Tick(fTimeDelta);
}

void CCharacter::Late_Tick(_float fTimeDelta)
{	
	CGameObject::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_DebugRender(m_pColliderCom))) return;

	Reset_Hit(fTimeDelta);
}

HRESULT CCharacter::Render()
{
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

void CCharacter::Set_TypeAnimIndex(_uint iAnimTag)
{
	if (m_pModelCom == nullptr)
		return;

	_int iAnimIndex = Find_TypeAnimIndex(iAnimTag);
	if (iAnimIndex == -1)
		return;

	m_eCurrentTypeAnimIndex = iAnimTag;
	m_pModelCom->Set_AnimationIndex(iAnimIndex);
}

void CCharacter::Camera_Zoom(_float3 vOffset)
{
	CTargetCamera* pCamera = dynamic_cast<CTargetCamera*>(m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER::LAYER_CAMERA]).front());

	if (XMVector3Equal(XMLoadFloat3(&vOffset), XMVectorZero()))
		pCamera->Reset_Offset();
	else
		pCamera->SetUp_Offset(vOffset);
}

void CCharacter::Camera_SetUp_LookAt_Hegith(_float fHeight)
{
	CTargetCamera* pCamera = dynamic_cast<CTargetCamera*>(m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER::LAYER_CAMERA]).front());

	if (fHeight == 0.f)
		pCamera->Reset_LookAt_Height();
	else
		pCamera->SetUp_LookAt_Height(fHeight);
}

HRESULT CCharacter::Init_Point_Light()
{
	return S_OK;
}

void CCharacter::Update_Light()
{
	if (m_pLight == nullptr)
		return;

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_u8[0] = m_pTransformCom->Get_Scaled().y;
	XMStoreFloat4(&m_pLight->Open_Light_Desc()->vPos, vPos);
}

HRESULT CCharacter::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
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

HRESULT CCharacter::Ready_Component()
{

	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = 0;
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom, &NavigationDesc))) return E_FAIL;

	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;
	if (FAILED(Add_Component<CStateMachine>(COM_STATEMACHINE_TAG, &m_pStateMachineCom))) return E_FAIL;

	CRigidBody::RIGIDBODY_DESC RigidBody_Desc = {};
	RigidBody_Desc.pOwner = this;
	if (FAILED(Add_Component<CRigidBody>(COM_RIGIDBODY_TAG, &m_pRigidBodyCom, &RigidBody_Desc))) return E_FAIL;

	return S_OK;
}

HRESULT CCharacter::Ready_Animation()
{
	return S_OK;
}

void CCharacter::Add_TypeAnimIndex(_uint iAnimTag, _uint iAnimIndex)
{
	_int iAnimIndx = Find_TypeAnimIndex(iAnimTag);
	if (iAnimIndx != -1)
		return;

	m_mapTypeAnimIndex.emplace(iAnimTag, iAnimIndex);
}

_int CCharacter::Find_TypeAnimIndex(_uint iAnimTag)
{
	auto& iter = m_mapTypeAnimIndex.find(iAnimTag);
	if (iter == m_mapTypeAnimIndex.end())
		return -1;

	return iter->second;
}

void CCharacter::Pushed()
{
	_vector vDir = XMLoadFloat3(&m_pColliderCom->Get_CollisionDir());
	_float fDist = m_pColliderCom->Get_PushedDist();

	vDir.m128_f32[1] = 0.f;

	m_pRigidBodyCom->Force(vDir, fDist * 10.f, CRigidBody::TYPE::TYPE_ACCEL);
}

void CCharacter::Pushed_Reset()
{
	m_pRigidBodyCom->Reset_Force(CRigidBody::TYPE::TYPE_ACCEL);
}

void CCharacter::Reset_Hit(_float fTimeDelta)
{
	if (!m_bHit_Effect)
		return;

	m_fHitCount += fTimeDelta;

	if (m_fHitCount > 0.5f)
	{
		m_fHitCount = 0.f;
		m_bHit_Effect = false;
	}
}

void CCharacter::Create_Damage_Effect(_float fLifeTime, const wstring& strTextureTag)
{
	CEffect::EFFECTINFO Info = {};
	Info.pOwner = this;
	Info.fLifeTime = fLifeTime;
	Info.strEffectTextureTag = strTextureTag;
	Info.vSize = _float2(0.1f, 0.1f);

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_f32[1] += m_pTransformCom->Get_Scaled().y;

	_vector vCamPos = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAM_POS);

	_vector vDir = XMVector3Normalize(vCamPos - vPos);
	vPos += vDir * m_pTransformCom->Get_Scaled().z * 0.5f;
	
	XMStoreFloat4(&Info.vPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTDAMAGE_TAG, &Info)))
		return;
}

void CCharacter::Create_Soul_Effect(_float fLifeTime)
{
	CEffect::EFFECTINFO Info = {};
	Info.pOwner = this;
	Info.fLifeTime = fLifeTime;
	Info.strEffectTextureTag = TEX_SOUL_TAG;

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_f32[1] += m_pTransformCom->Get_Scaled().y * 2.f;

	XMStoreFloat4(&Info.vPos, vPos);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTSOUL_TAG, &Info)))
		return;
}

void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStateMachineCom);

	Safe_Release(m_pLight);
	
}

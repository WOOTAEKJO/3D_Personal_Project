#include "stdafx.h"
#include "..\Public\Plateform_Trap.h"

#include "Bone.h"

CPlateform_Trap::CPlateform_Trap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CPlateform_Trap::CPlateform_Trap(const CPlateform_Trap& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlateform_Trap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlateform_Trap::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		PLATEFORM_TRAP_DESC* TrapDesc = (PLATEFORM_TRAP_DESC*)pArg;

		m_strModelTag = TrapDesc->strModelTag;
		m_fAngle = TrapDesc->fAngle;
		m_fAmplitude = TrapDesc->fAmplitude;

		if (FAILED(Ready_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMLoadFloat4(&TrapDesc->vPos));
		m_pTransformCom->Set_Scaling(2.f, 2.f, 2.f);
		m_pTransformCom->Rotation(m_pTransformCom->Get_State(CTransform::STATE::STATE_UP), m_fAngle.y);

		if (FAILED(m_pGameInstance->Add_Collision(COLLIDER_LAYER::COL_TRAP, m_pColliderCom)))
			return E_FAIL;
	}


	return S_OK;
}

void CPlateform_Trap::Priority_Tick(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Matrix());
}

void CPlateform_Trap::Tick(_float fTimeDelta)
{
	Pendulum_Movement(fTimeDelta);
}

void CPlateform_Trap::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CPlateform_Trap::Render()
{
	if (m_pModelCom == nullptr)
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (_uint i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(SHADER_TBN::TBN_MODEL);

		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void CPlateform_Trap::Write_Json(json& Out_Json)
{
	string strTag;

	strTag.assign(m_strModelTag.begin(), m_strModelTag.end());

	Out_Json.emplace("ModelTag", strTag);
	CGameObject::Write_Json(Out_Json);
}

void CPlateform_Trap::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("ModelTag") == In_Json.end())
		return;

	string strTag  = In_Json["ModelTag"];

	m_strModelTag.assign(strTag.begin(), strTag.end());

	CGameObject::Load_FromJson(In_Json);

	if (FAILED(Ready_Component()))
		return;
}

HRESULT CPlateform_Trap::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos",
		&m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlateform_Trap::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;
	CBounding_OBB::BOUNDING_OBB_DESC OBB_Desc = {};
	OBB_Desc.pOnwer = this;
	OBB_Desc.eType = CBounding::TYPE::TYPE_OBB;
	OBB_Desc.bUseCol = true;
	OBB_Desc.vExtents = _float3(0.1f, 0.1f, 0.3f);
	OBB_Desc.vCenter = _float3(0.f, -OBB_Desc.vExtents.y * 14.f, 0.f);
	OBB_Desc.vRotation = _float3(0.f, 0.f, 0.f);
	if (FAILED(Add_Component<CCollider>(COM_COLLIDER_TAG, &m_pColliderCom, &OBB_Desc))) return E_FAIL;

	return S_OK;
}

void CPlateform_Trap::Pendulum_Movement(_float fTimeDelta)
{
	_float fGravity = 9.81f;
	_float fLength = 1.f;

	_float fAccelerationZ = -(fGravity / fLength) * sin(m_fAngle.z);

	m_fAngularVelocity.z += fAccelerationZ * fTimeDelta;
	m_fAngle.z += m_fAngularVelocity.z * fTimeDelta;

	m_fAngle.z = m_fAmplitude * sin(m_fAngle.z);

	m_pTransformCom->Rotation_Quaternio(m_fAngle.x, m_fAngle.y, m_fAngle.z);
}

CPlateform_Trap* CPlateform_Trap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlateform_Trap* pInstance = new CPlateform_Trap(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlateform_Trap");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlateform_Trap::Clone(void* pArg)
{
	CPlateform_Trap* pInstance = new CPlateform_Trap(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlateform_Trap");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlateform_Trap::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	
}

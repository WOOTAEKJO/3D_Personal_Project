#include "stdafx.h"
#include "..\Public\Plateform.h"

CPlateform::CPlateform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CPlateform::CPlateform(const CPlateform& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlateform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlateform::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		/*OBDEMOVALUE* ObjectDemoValue = (OBDEMOVALUE*)pArg;

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, ObjectDemoValue->vPos);
		m_strModelTag = ObjectDemoValue->strModelTag;

		if (FAILED(Ready_Component()))
			return E_FAIL;*/
		m_strModelTag = ((PLATEFORM_DESC*)pArg)->strModelTag;

		if (FAILED(Ready_Component()))
			return E_FAIL;
	}

	m_RandomNumber = mt19937_64(m_RandomDevice());
	
	return S_OK;
}

void CPlateform::Priority_Tick(_float fTimeDelta)
{
}

void CPlateform::Tick(_float fTimeDelta)
{
	if (m_bFall)
	{
		Fall(fTimeDelta);

		if (m_pTransformCom->Get_State(CTransform::STATE::STATE_POS).m128_u8[1] <= 0.f)
			Set_Dead();
	}
	else if(!m_bFall && (m_iTriggerNum < 0)) {
		uniform_real_distribution<float>	RandomAngle(10.f, 30.f);
		m_fAngle.x = RandomAngle(m_RandomNumber);
		m_fAngle.z = RandomAngle(m_RandomNumber);
	}
}

void CPlateform::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CPlateform::Render()
{
	if (m_pModelCom == nullptr)
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (_uint i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(1);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CPlateform::Write_Json(json& Out_Json)
{
	string strTag;

	strTag.assign(m_strModelTag.begin(), m_strModelTag.end());

	Out_Json.emplace("ModelTag", strTag);
	CGameObject::Write_Json(Out_Json);
}

void CPlateform::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("ModelTag") == In_Json.end())
		return;

	string strTag  = In_Json["ModelTag"];

	m_strModelTag.assign(strTag.begin(), strTag.end());

	CGameObject::Load_FromJson(In_Json);

	if (FAILED(Ready_Component()))
		return;
}

HRESULT CPlateform::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlateform::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(m_strModelTag, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

void CPlateform::Fall(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	_float fGravity = 9.81f;
	_float fLength = 1.f;

	_float fAccelerationZ = -(fGravity / fLength) * sin(m_fAngle.z);
	_float fAccelerationX = -(fGravity / fLength) * sin(m_fAngle.x);

	m_fAngularVelocity.z += fAccelerationZ * fTimeDelta * 3.f;
	m_fAngularVelocity.x += fAccelerationX * fTimeDelta * 3.f;
	m_fAngle.z += m_fAngularVelocity.z * fTimeDelta * 3.f;
	m_fAngle.x += m_fAngularVelocity.x * fTimeDelta * 3.f;

	m_fAngle.z = m_fAmplitude * sin(m_fAngle.z);
	m_fAngle.x = m_fAmplitude * cos(m_fAngle.x);

	m_pTransformCom->Rotation_Quaternio(m_fAngle.x, m_fAngle.y, m_fAngle.z);
	
	if (m_fTimeAcc > 1.5f)
	{
		_vector vUp = m_pTransformCom->Get_State(CTransform::STATE::STATE_UP);
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

		vPos += XMVector3Normalize(-vUp) * fTimeDelta * 2.f;

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	}
}

CPlateform* CPlateform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlateform* pInstance = new CPlateform(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlateform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlateform::Clone(void* pArg)
{
	CPlateform* pInstance = new CPlateform(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlateform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlateform::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

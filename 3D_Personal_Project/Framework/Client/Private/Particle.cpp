#include "..\Public\Particle.h"

#include "GameInstance.h"
#include "Bone.h"

CParticle::CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CParticle::CParticle(const CParticle& rhs)
	:CGameObject(rhs)
{
}

HRESULT CParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	PARTICLEINFO* pInfo = (PARTICLEINFO*)pArg;
	m_strParticleTag = pInfo->strParticleTag;
	m_fLifeTime = pInfo->fLifeTime;

	if (FAILED(m_pGameInstance->Load_Data_Json(m_strParticleTag + TEXT("Info"), this)))
		return E_FAIL;

	return S_OK;
}

void CParticle::Priority_Tick(_float fTimeDelta)
{
}

void CParticle::Tick(_float fTimeDelta)
{
	//if (m_bUpdate)
	
	m_pBufferCom->Update(fTimeDelta);
}

void CParticle::Late_Tick(_float fTimeDelta)
{
	if (m_pSocketBone != nullptr)
	{
		XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() *
			m_pSocketBone->Get_CombinedTransformationMatrix() * m_pOwnerTransform->Get_WorldMatrix_Matrix());
		/*_vector vPos = XMVectorSet(m_matWorldMat.m[3][0], m_matWorldMat.m[3][1], m_matWorldMat.m[3][2], 1.f);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
		
		_vector vLook = m_pOwnerTransform->Get_State(CTransform::STATE::STATE_LOOK);
		vLook.m128_f32[1] = 0.f;
		m_pTransformCom->LookAt(vLook);*/
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLEND, this)))
		return;
}

HRESULT CParticle::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iIndx = 0;
	if (m_pBufferCom->Open_InstancingDesc()->eColorType == INSTANCING_DESC::COLORTYPE::NORMAL_COLOR)
		iIndx = 0;
	else if (m_pBufferCom->Open_InstancingDesc()->eColorType == INSTANCING_DESC::COLORTYPE::SOLID_COLOR)
		iIndx = 1;
	else if (m_pBufferCom->Open_InstancingDesc()->eColorType == INSTANCING_DESC::COLORTYPE::BLEND_COLOR)
		iIndx = 2;

	m_pShaderCom->Begin(iIndx);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

void CParticle::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("ModelTag") == In_Json.end())
		return;

	m_iSocketBoneIndex = In_Json["BoneIndex"];
	//string strTag = In_Json["ModelTag"];

	//m_strParticleTag.assign(strTag.begin(), strTag.end());

	CGameObject::Load_FromJson(In_Json);
}

HRESULT CParticle::Bind_ShaderResources()
{
	/*if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;*/
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorldMat)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraPos", &m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS),
		sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraLook", &m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_LOOK),
		sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_PARTICLEPOINT_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CVIBuffer_Particle_Point>(m_strParticleTag, &m_pBufferCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_pBufferCom->Open_InstancingDesc()->strTextureTag, &m_pTextureCom))) return E_FAIL;
	
	return S_OK;
}

void CParticle::Judge_Dead(_float fTimeDelta)
{
	if (m_pBufferCom->Open_InstancingDesc()->bLoop == false)
	{
		m_fTimeAcc += fTimeDelta;

		if (m_fTimeAcc > m_fLifeTime)
			Set_Dead();
	}
}

void CParticle::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferCom);

	Safe_Release(m_pSocketBone);
	Safe_Release(m_pOwnerTransform);
}

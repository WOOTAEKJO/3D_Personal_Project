#include "..\Public\Particle_Stage.h"

#include "GameInstance.h"
#include "Bone.h"
#include "Player.h"

CParticle_Stage::CParticle_Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticle(pDevice, pContext)
{
}

CParticle_Stage::CParticle_Stage(const CParticle_Stage& rhs)
	: CParticle(rhs)
{
}

HRESULT CParticle_Stage::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Stage::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	STAGEPARTICLE_DESC* pInfo = (STAGEPARTICLE_DESC*)pArg;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, pInfo->vPos);
	m_pOwner = pInfo->pOwner;

	return S_OK;
}

void CParticle_Stage::Priority_Tick(_float fTimeDelta)
{
	m_pTransformCom;
}

void CParticle_Stage::Tick(_float fTimeDelta)
{

	m_pBufferCom->Update(fTimeDelta);
}

void CParticle_Stage::Late_Tick(_float fTimeDelta)
{
	if (m_bBlur) {
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
			return;
	}
	else {
		Compute_CamDistance();
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLEND, this)))
			return;
	}
}

HRESULT CParticle_Stage::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Stage::Load_FromJson(const json& In_Json)
{
	
	__super::Load_FromJson(In_Json);
}

HRESULT CParticle_Stage::Bind_ShaderResources()
{
	
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
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

HRESULT CParticle_Stage::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	
	return S_OK;
}

CParticle_Stage* CParticle_Stage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Stage* pInstance = new CParticle_Stage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Stage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle_Stage::Clone(void* pArg)
{
	CParticle_Stage* pInstance = new CParticle_Stage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Stage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle_Stage::Free()
{
	__super::Free();
}

#include "..\Public\Particle_Sub.h"

#include "GameInstance.h"
#include "Bone.h"
#include "Player.h"

CParticle_Sub::CParticle_Sub(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticle(pDevice, pContext)
{
}

CParticle_Sub::CParticle_Sub(const CParticle_Sub& rhs)
	: CParticle(rhs)
{
}

HRESULT CParticle_Sub::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Sub::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	PARTICLEINFO* pInfo = (PARTICLEINFO*)pArg;

	m_pOwner = pInfo->pOwner;
	if (m_pOwner == nullptr)
		return E_FAIL;

	m_pOwnerTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pOwnerTransform);
	
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, m_pOwnerTransform->Get_State(CTransform::STATE::STATE_POS));

	return S_OK;
}

void CParticle_Sub::Priority_Tick(_float fTimeDelta)
{
}

void CParticle_Sub::Tick(_float fTimeDelta)
{
	/*m_fTime += fTimeDelta;

	if (m_fTime < 0.5f)
	{
		
	}*/

	if (m_pOwner != nullptr && !m_pOwner->Get_Dead())
	{
		//m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, m_pOwnerTransform->Get_State(CTransform::STATE::STATE_POS));
		//m_pTransformCom->Towards_Target(m_pOwnerTransform->Get_State(CTransform::STATE::STATE_POS), fTimeDelta * 1.f, 0.01f);
		//m_pTransformCom->LookAt(m_pOwnerTransform->Get_State(CTransform::STATE::STATE_POS));
	}

	m_pBufferCom->Update(fTimeDelta);
}

void CParticle_Sub::Late_Tick(_float fTimeDelta)
{
	
	/*if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
		return;*/
	Compute_CamDistance();
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLEND, this)))
		return;
	if(m_pOwner == nullptr || m_pOwner->Get_Dead())
		Judge_Dead(fTimeDelta);
}

HRESULT CParticle_Sub::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Sub::Load_FromJson(const json& In_Json)
{
	
	__super::Load_FromJson(In_Json);
}

HRESULT CParticle_Sub::Bind_ShaderResources()
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

HRESULT CParticle_Sub::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	
	return S_OK;
}

CParticle_Sub* CParticle_Sub::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Sub* pInstance = new CParticle_Sub(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Sub");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle_Sub::Clone(void* pArg)
{
	CParticle_Sub* pInstance = new CParticle_Sub(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Sub");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle_Sub::Free()
{
	__super::Free();
}

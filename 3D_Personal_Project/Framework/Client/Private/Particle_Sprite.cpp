#include "..\Public\Particle_Sprite.h"

#include "GameInstance.h"
#include "Bone.h"
#include "Player.h"

CParticle_Sprite::CParticle_Sprite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticle(pDevice, pContext)
{
}

CParticle_Sprite::CParticle_Sprite(const CParticle_Sprite& rhs)
	: CParticle(rhs)
{
}

HRESULT CParticle_Sprite::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Sprite::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_Data_Json(m_strParticleTag + TEXT("Info"), this)))
		return E_FAIL;

	PARTICLEINFO* pInfo = (PARTICLEINFO*)pArg;

	m_pOwner = pInfo->pOwner;
	if (m_pOwner == nullptr)
		return E_FAIL;

	m_pOwnerTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pOwnerTransform);
	//m_pSocketBone = dynamic_cast<CPlayer*>(m_pOwner)->Get_BodyModel()->Get_Bone(m_iSocketBoneIndex);
	m_pSocketBone = pInfo->pBones[m_iSocketBoneIndex];
	Safe_AddRef(m_pSocketBone);

	XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() *
			m_pSocketBone->Get_CombinedTransformationMatrix() * m_pOwnerTransform->Get_WorldMatrix_Matrix());

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fMaxFrame = m_pTextureCom->Get_TextureNum();
	m_fFrame = (_float)(rand() % (_uint)m_fMaxFrame);
	// 스프라이트 파티클을 위함

	return S_OK;
}

void CParticle_Sprite::Priority_Tick(_float fTimeDelta)
{
	
}

void CParticle_Sprite::Tick(_float fTimeDelta)
{	
	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fMaxFrame <= m_fFrame)
		m_fFrame = 0.f;

	m_pBufferCom->Update(fTimeDelta);
}

void CParticle_Sprite::Late_Tick(_float fTimeDelta)
{
	if (m_pSocketBone != nullptr)
	{
		/*XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() *
			m_pSocketBone->Get_CombinedTransformationMatrix() * m_pOwnerTransform->Get_WorldMatrix_Matrix());

		_vector vPos = XMVectorSet(m_matWorldMat.m[3][0], m_matWorldMat.m[3][1], m_matWorldMat.m[3][2], 1.f);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
		
		_vector vLook = m_pOwnerTransform->Get_State(CTransform::STATE::STATE_LOOK);
		
		m_pTransformCom->LookAt_Dir(vLook);*/
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
		return;

	Judge_Dead(fTimeDelta);
}

HRESULT CParticle_Sprite::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Sprite::Load_FromJson(const json& In_Json)
{
	
	__super::Load_FromJson(In_Json);
}

HRESULT CParticle_Sprite::Bind_ShaderResources()
{

	/*if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;*/
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorldMat)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_fFrame)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraPos", &m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS),
		sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraLook", &m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_LOOK),
		sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Sprite::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	
	return S_OK;
}

CParticle_Sprite* CParticle_Sprite::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Sprite* pInstance = new CParticle_Sprite(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Sprite");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle_Sprite::Clone(void* pArg)
{
	CParticle_Sprite* pInstance = new CParticle_Sprite(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Sprite");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle_Sprite::Free()
{
	__super::Free();
}

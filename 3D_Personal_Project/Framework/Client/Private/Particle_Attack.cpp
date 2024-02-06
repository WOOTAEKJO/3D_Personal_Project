#include "..\Public\Particle_Attack.h"

#include "GameInstance.h"
#include "Bone.h"
#include "Player.h"

CParticle_Attack::CParticle_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticle(pDevice, pContext)
{
}

CParticle_Attack::CParticle_Attack(const CParticle_Attack& rhs)
	: CParticle(rhs)
{
}

HRESULT CParticle_Attack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Attack::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	ATTACKPARTICLE_DESC* pInfo = (ATTACKPARTICLE_DESC*)pArg;

	m_pOwner = pInfo->pOwner;
	if (m_pOwner == nullptr)
		return E_FAIL;

	m_pOwnerTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pOwnerTransform);

	m_vPos = pInfo->vPos;
	m_vDir = pInfo->vDir;
	m_bKeep = pInfo->bKeep;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, m_vPos);

	if (!XMVector3Equal(XMLoadFloat3(&m_vDir), XMVectorZero()))
	{
		_vector vUp = m_pTransformCom->Get_State(CTransform::STATE::STATE_UP);
		_float fAngle = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vUp), XMVector3Normalize(XMLoadFloat3(&m_vDir))));

		m_pTransformCom->Rotation_Quaternio(fAngle, fAngle, fAngle);
	}

	m_fMaxFrame = m_pTextureCom->Get_TextureNum();

	if (m_fMaxFrame > 1)
	{
		m_bSprite = true;
		m_fFrame = (_float)(rand() % (_uint)m_fMaxFrame);
	}

	return S_OK;
}

void CParticle_Attack::Priority_Tick(_float fTimeDelta)
{
	/*if (m_bKeep)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, m_vPos);

		if (m_pOwner == nullptr || m_pOwner->Get_Dead())
			Set_Dead();

	}
	else
	{
		
	}*/
}

void CParticle_Attack::Tick(_float fTimeDelta)
{
	if (m_bSprite)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta;

		if (m_fMaxFrame <= m_fFrame)
			m_fFrame = 0.f;
	}

	m_pBufferCom->Update(fTimeDelta);
}

void CParticle_Attack::Late_Tick(_float fTimeDelta)
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
	Judge_Dead(fTimeDelta);
	
}

HRESULT CParticle_Attack::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Attack::Load_FromJson(const json& In_Json)
{
	
	__super::Load_FromJson(In_Json);
}

HRESULT CParticle_Attack::Bind_ShaderResources()
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

HRESULT CParticle_Attack::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	
	return S_OK;
}

CParticle_Attack* CParticle_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Attack* pInstance = new CParticle_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Attack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle_Attack::Clone(void* pArg)
{
	CParticle_Attack* pInstance = new CParticle_Attack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Attack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle_Attack::Free()
{
	__super::Free();
}

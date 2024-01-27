#include "..\Public\Particle_Always.h"

#include "GameInstance.h"
#include "Bone.h"
#include "Player.h"

CParticle_Always::CParticle_Always(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticle(pDevice, pContext)
{
}

CParticle_Always::CParticle_Always(const CParticle_Always& rhs)
	: CParticle(rhs)
{
}

HRESULT CParticle_Always::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Always::Initialize(void* pArg)
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
	if (pInfo->pBones[m_iSocketBoneIndex] != nullptr)
	{
		m_pSocketBone = pInfo->pBones[m_iSocketBoneIndex];
		Safe_AddRef(m_pSocketBone);

		XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() *
			m_pSocketBone->Get_CombinedTransformationMatrix() * m_pOwnerTransform->Get_WorldMatrix_Matrix());
	}

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Always::Priority_Tick(_float fTimeDelta)
{
}

void CParticle_Always::Tick(_float fTimeDelta)
{

	m_pBufferCom->Update(fTimeDelta);
}

void CParticle_Always::Late_Tick(_float fTimeDelta)
{
	if (m_pSocketBone != nullptr)
	{
		XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() *
			m_pSocketBone->Get_CombinedTransformationMatrix() * m_pOwnerTransform->Get_WorldMatrix_Matrix());
	}
	else
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, m_vPos);
		XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix());
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
		return;

	if (m_pOwner == nullptr || m_pOwner->Get_Dead())
	{
		m_fTimeAcc += fTimeDelta;
		if(m_fTimeAcc > m_fLifeTime)
			Set_Dead();
	}
}

HRESULT CParticle_Always::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Always::Load_FromJson(const json& In_Json)
{
	
	__super::Load_FromJson(In_Json);
}

HRESULT CParticle_Always::Bind_ShaderResources()
{
	
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Always::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	
	return S_OK;
}

CParticle_Always* CParticle_Always::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Always* pInstance = new CParticle_Always(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Always");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle_Always::Clone(void* pArg)
{
	CParticle_Always* pInstance = new CParticle_Always(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Always");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle_Always::Free()
{
	__super::Free();
}

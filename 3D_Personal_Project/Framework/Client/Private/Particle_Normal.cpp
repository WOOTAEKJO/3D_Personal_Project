#include "..\Public\Particle_Normal.h"

#include "GameInstance.h"
#include "Bone.h"
#include "Player.h"

CParticle_Normal::CParticle_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticle(pDevice, pContext)
{
}

CParticle_Normal::CParticle_Normal(const CParticle_Normal& rhs)
	: CParticle(rhs)
{
}

HRESULT CParticle_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Normal::Initialize(void* pArg)
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
	m_pSocketBone = pInfo->pBones[m_iSocketBoneIndex];
	Safe_AddRef(m_pSocketBone);

	/*m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, m_pOwnerTransform->Get_State(CTransform::STATE::STATE_POS));

	_vector vLook = m_pOwnerTransform->Get_State(CTransform::STATE::STATE_LOOK);

	m_pTransformCom->LookAt_Dir(vLook);*/
	XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() *
			m_pSocketBone->Get_CombinedTransformationMatrix() * m_pOwnerTransform->Get_WorldMatrix_Matrix());

		/*_vector vPos = XMVectorSet(m_matWorldMat.m[3][0], m_matWorldMat.m[3][1], m_matWorldMat.m[3][2], 1.f);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);

		_vector vLook = m_pOwnerTransform->Get_State(CTransform::STATE::STATE_LOOK);

		m_pTransformCom->LookAt_Dir(vLook);*/

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Normal::Priority_Tick(_float fTimeDelta)
{
}

void CParticle_Normal::Tick(_float fTimeDelta)
{
	//if (m_bUpdate)
	
	m_pBufferCom->Update(fTimeDelta);
}

void CParticle_Normal::Late_Tick(_float fTimeDelta)
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

HRESULT CParticle_Normal::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Normal::Load_FromJson(const json& In_Json)
{
	
	__super::Load_FromJson(In_Json);
}

HRESULT CParticle_Normal::Bind_ShaderResources()
{
	
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Normal::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;
	
	return S_OK;
}

CParticle_Normal* CParticle_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Normal* pInstance = new CParticle_Normal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Normal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle_Normal::Clone(void* pArg)
{
	CParticle_Normal* pInstance = new CParticle_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Normal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle_Normal::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "..\Public\Effect_Laser.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_Laser::CEffect_Laser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Laser::CEffect_Laser(const CEffect_Laser& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Laser::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Laser::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	EFFECT_LASERINFO* Info = (EFFECT_LASERINFO*)pArg;

	m_vSolid_Color = Info->vColor;
	if (Info->pTarger == nullptr)
		return E_FAIL;
	m_pTargetTransform = Info->pTarger->Get_Component<CTransform>();
	if (m_pTargetTransform == nullptr)
		return E_FAIL;

	m_fMaxFrame = m_pTextureCom->Get_TextureNum();

	if (m_fMaxFrame > 1)
	{
		m_bSprite = true;
		m_fFrame = (_float)(rand() % (_uint)m_fMaxFrame);

	}

	m_pDBufferCom->Update_Vertex(CVIBuffer_DRect::DIRECTION::POSTIVE_Y);

	return S_OK;
}

void CEffect_Laser::Priority_Tick(_float fTimeDelta)
{

	ReSize();

	//__super::Priority_Tick(fTimeDelta);

	Update_World();
}

void CEffect_Laser::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEffect_Laser::Late_Tick(_float fTimeDelta)
{

	//__super::Late_Tick(fTimeDelta);
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
		return;

	if (m_pOwner == nullptr || m_pOwner->Get_Dead())
		Set_Dead();

}

HRESULT CEffect_Laser::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(5);

	m_pDBufferCom->Bind_Buffer();

	m_pDBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Laser::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	/*if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;*/

	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorld)))
		return E_FAIL;*/
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(RTV_TYPE::RT_DEPTH, m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Laser::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_POS_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_strTextureTag, &m_pTextureCom))) return E_FAIL;
	if (FAILED(Add_Component<CVIBuffer_DRect>(BUFFER_DRECT_TAG, &m_pDBufferCom))) return E_FAIL;

	return S_OK;
}

void CEffect_Laser::ReSize()
{
	if (m_pTargetTransform == nullptr)
		return;

	_vector vTargetPos = m_pTargetTransform->Get_State(CTransform::STATE::STATE_POS);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	m_vSize.y = XMVectorGetX(XMVector3Length(vTargetPos - vPos)) * 2.f;

	m_vSize.x = 0.1f;
	m_vSize.z = 1.f;

	//m_pTransformCom->Set_Scaling(m_vSize.x, m_vSize.y, m_vSize.z);
}

void CEffect_Laser::Update_World()
{
	if (m_pTargetTransform == nullptr)
		return;

	CTransform* pOwnerTransform = m_pOwner->Get_Component<CTransform>();

	_vector vTargetUp = XMVector3Normalize(pOwnerTransform->Get_State(CTransform::STATE::STATE_UP));
	_vector vTargetLook = XMVector3Normalize(pOwnerTransform->Get_State(CTransform::STATE::STATE_LOOK));
	_vector vTargetRight = XMVector3Normalize(pOwnerTransform->Get_State(CTransform::STATE::STATE_RIGHT));

	m_pTransformCom->Set_State(CTransform::STATE::STATE_RIGHT, XMVectorSetW(vTargetRight * m_vSize.x, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE::STATE_UP, XMVectorSetW(vTargetLook * m_vSize.y, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE::STATE_LOOK, XMVectorSetW(vTargetUp * m_vSize.z, 0.f));
	
}

CEffect_Laser* CEffect_Laser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Laser* pInstance = new CEffect_Laser(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Laser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Laser::Clone(void* pArg)
{
	CEffect_Laser* pInstance = new CEffect_Laser(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Laser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Laser::Free()
{
	__super::Free();

}

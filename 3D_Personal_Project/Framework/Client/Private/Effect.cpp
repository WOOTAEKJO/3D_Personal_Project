#include "stdafx.h"
#include "..\Public\Effect.h"

#include "GameInstance.h"
#include "Character.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CAlphaObject(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect& rhs)
	: CAlphaObject(rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	EFFECTINFO* pInfo = (EFFECTINFO*)pArg;

	m_strTextureTag = pInfo->strEffectTextureTag;
	m_pOwner = pInfo->pOwner;
	/*if (m_pOwner == nullptr)
		return E_FAIL;*/

	m_fLifeTime = pInfo->fLifeTime;
	m_vSize = pInfo->vSize;
	m_vPos = pInfo->vPos;
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMLoadFloat4(&m_vPos));

	return S_OK;
}

void CEffect::Priority_Tick(_float fTimeDelta)
{
	if (m_bSprite)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta;

		if (m_fMaxFrame <= m_fFrame)
			m_fFrame = 0.f;
	}

	m_pTransformCom->LookAt(m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_POS));
	
	__super::Priority_Tick(fTimeDelta);
}

void CEffect::Tick(_float fTimeDelta)
{
	
	__super::Tick(fTimeDelta);
}

void CEffect::Late_Tick(_float fTimeDelta)
{
	

	__super::Late_Tick(fTimeDelta);
	Compute_CamDistance();
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLEND, this)))
		return;
}

HRESULT CEffect::Render()
{

	m_pShaderCom->Begin(1);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
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

HRESULT CEffect::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_POS_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_strTextureTag, &m_pTextureCom))) return E_FAIL;
	if (FAILED(Add_Component<CVIBuffer_Rect>(BUFFER_RECT_TAG, &m_pBufferCom))) return E_FAIL;

	return S_OK;
}

void CEffect::Judge_Dead(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > m_fLifeTime)
		Set_Dead();
}

void CEffect::Size_Up(_float fTimeDelta)
{
	_float3 fSize = m_pTransformCom->Get_Scaled();

	fSize.x += fTimeDelta;
	fSize.y += fTimeDelta;
	fSize.z += fTimeDelta;

	m_pTransformCom->Set_Scaling(fSize.x, fSize.y, fSize.z);
}

void CEffect::Invisibility(_float fTimeDelta)
{
	m_fAlpha += fTimeDelta;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferCom);	
	Safe_Release(m_pDBufferCom);
}

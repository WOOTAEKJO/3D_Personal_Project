#include "stdafx.h"
#include "..\Public\Effect_Energy.h"

#include "GameInstance.h"
#include "Character.h"

#include "Utility_Effect.h"

CEffect_Energy::CEffect_Energy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Energy::CEffect_Energy(const CEffect_Energy& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Energy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Energy::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	EFFECT_ENERGYINFO* Info = (EFFECT_ENERGYINFO*)pArg;

	m_vSolid_Color = Info->vColor;
	m_fSizeSpeed = Info->fSizeSpeed;

	m_fMaxFrame = m_pTextureCom->Get_TextureNum();

	if (m_fMaxFrame > 1)
	{
		m_bSprite = true;
		m_fFrame = (_float)(rand() % (_uint)m_fMaxFrame);

	}

	m_pTransformCom->Set_Scaling(m_vSize.x, m_vSize.y, 0.1f);

	return S_OK;
}

void CEffect_Energy::Priority_Tick(_float fTimeDelta)
{

	Update_Pos();
	__super::Priority_Tick(fTimeDelta);
}

void CEffect_Energy::Tick(_float fTimeDelta)
{
	Size_Up(fTimeDelta * m_fSizeSpeed);
	__super::Tick(fTimeDelta);
}

void CEffect_Energy::Late_Tick(_float fTimeDelta)
{
	//Judge_Dead(fTimeDelta);

	__super::Late_Tick(fTimeDelta);

	if (m_pOwner == nullptr || m_pOwner->Get_Dead())
		Set_Dead();

}

HRESULT CEffect_Energy::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Energy::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Energy::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Energy::Update_Pos()
{
	_vector vOwnerPos = m_pOwner->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS);

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vOwnerPos);
}

CEffect_Energy* CEffect_Energy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Energy* pInstance = new CEffect_Energy(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Energy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Energy::Clone(void* pArg)
{
	CEffect_Energy* pInstance = new CEffect_Energy(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Energy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Energy::Free()
{
	__super::Free();

}

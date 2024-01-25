#include "stdafx.h"
#include "..\Public\Effect_Reaper.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_Reaper::CEffect_Reaper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Reaper::CEffect_Reaper(const CEffect_Reaper& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Reaper::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Reaper::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_fMaxFrame = m_pTextureCom->Get_TextureNum();

	if (m_fMaxFrame > 1)
	{
		m_bSprite = true;
		m_fFrame = (_float)(rand() % (_uint)m_fMaxFrame);

	}
	m_pTransformCom->Rotation_Quaternio(XMConvertToRadians(90.f), 0.f, 0.f);
	m_pTransformCom->Set_Scaling(m_vSize.x, m_vSize.y, 0.1f);
	
	m_vSolid_Color = _float4(1.f,0.6f,0.4f,1.f);

	m_fAlpha = 0.6f;
	return S_OK;
}

void CEffect_Reaper::Priority_Tick(_float fTimeDelta)
{

	//__super::Priority_Tick(fTimeDelta);
}

void CEffect_Reaper::Tick(_float fTimeDelta)
{
	//m_fRadius += fTimeDelta;
	__super::Tick(fTimeDelta);
}

void CEffect_Reaper::Late_Tick(_float fTimeDelta)
{
	Judge_Dead(fTimeDelta);

	__super::Late_Tick(fTimeDelta);

}

HRESULT CEffect_Reaper::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(4);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Reaper::Bind_ShaderResources()
{
	_float4 vPos = {};
	XMStoreFloat4(&vPos,m_pOwner->Get_Component<CTransform>()->Get_State(CTransform::STATE::STATE_POS));

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCenter", &vPos, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRadius", &m_fRadius, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Reaper::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

CEffect_Reaper* CEffect_Reaper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Reaper* pInstance = new CEffect_Reaper(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Reaper");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Reaper::Clone(void* pArg)
{
	CEffect_Reaper* pInstance = new CEffect_Reaper(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Reaper");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Reaper::Free()
{
	__super::Free();

}

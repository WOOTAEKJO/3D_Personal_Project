#include "stdafx.h"
#include "..\Public\Effect_Soul.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_Soul::CEffect_Soul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Soul::CEffect_Soul(const CEffect_Soul& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Soul::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Soul::Initialize(void* pArg)
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

	m_pTransformCom->Set_Scaling(0.1f, 0.1f, 0.1f);

	return S_OK;
}

void CEffect_Soul::Priority_Tick(_float fTimeDelta)
{

	__super::Priority_Tick(fTimeDelta);
}

void CEffect_Soul::Tick(_float fTimeDelta)
{
	Size_Up(fTimeDelta * 1.5f);
	Invisibility(fTimeDelta * 3.f);
	Upward(fTimeDelta);
	__super::Tick(fTimeDelta);
}

void CEffect_Soul::Late_Tick(_float fTimeDelta)
{
	Judge_Dead(fTimeDelta);

	__super::Late_Tick(fTimeDelta);

}

HRESULT CEffect_Soul::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(3);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

void CEffect_Soul::Upward(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vUp = m_pTransformCom->Get_State(CTransform::STATE::STATE_UP);

	vPos += XMVector3Normalize(vUp) * fTimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
}

HRESULT CEffect_Soul::Bind_ShaderResources()
{
	

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CEffect_Soul::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

CEffect_Soul* CEffect_Soul::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Soul* pInstance = new CEffect_Soul(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Soul");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Soul::Clone(void* pArg)
{
	CEffect_Soul* pInstance = new CEffect_Soul(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Soul");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Soul::Free()
{
	__super::Free();

}

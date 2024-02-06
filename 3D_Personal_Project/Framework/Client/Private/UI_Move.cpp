#include "stdafx.h"
#include "..\Public\UI_Move.h"

#include "GameInstance.h"

CUI_Move::CUI_Move(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
}

CUI_Move::CUI_Move(const CUI_Move& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Move::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Move::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	UI_MOVE_DESC* pDesc = (UI_MOVE_DESC*)pArg;

	m_fSpeed = pDesc->fSpeed;
	m_fAngle = pDesc->fAngle;
	m_vDir = pDesc->vDir;

	return S_OK;
}

void CUI_Move::Priority_Tick(_float fTimeDelta)
{
}

void CUI_Move::Tick(_float fTimeDelta)
{
	Move(fTimeDelta);
}

void CUI_Move::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CUI_Move::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Move::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Move::Ready_Component()
{
	if (FAILED(__super::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CUI_Move::Move(_float fTimeDelta)
{
	if (m_fSpeed != 0.f)
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
		vPos += XMLoadFloat3(&m_vDir) * m_fSpeed * fTimeDelta;

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	}
	
	if (m_fAngle != 0.f)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta * m_fAngle);
	}

}

CUI_Move* CUI_Move::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Move* pInstance = new CUI_Move(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Move");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Move::Clone(void* pArg)
{
	CUI_Move* pInstance = new CUI_Move(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Move::Free()
{
	__super::Free();

}

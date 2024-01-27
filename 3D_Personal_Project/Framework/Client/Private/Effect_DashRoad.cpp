#include "stdafx.h"
#include "..\Public\Effect_DashRoad.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_DashRoad::CEffect_DashRoad(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_DashRoad::CEffect_DashRoad(const CEffect_DashRoad& rhs)
	: CEffect(rhs),m_pSpare(rhs.m_pSpare)
{
}

HRESULT CEffect_DashRoad::Initialize_Prototype()
{
	m_pSpare = new _float;

	*m_pSpare = 0.f;

	return S_OK;
}

HRESULT CEffect_DashRoad::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	EFFECT_DASHROADINFO* Info = (EFFECT_DASHROADINFO*)pArg;

	m_vSolid_Color = Info->vColor;
	
	m_fMaxFrame = m_pTextureCom->Get_TextureNum();

	if (m_fMaxFrame > 1)
	{
		m_bSprite = true;
		m_fFrame = (_float)(rand() % (_uint)m_fMaxFrame);

	}

	m_pDBufferCom->Update_Vertex(CVIBuffer_DRect::DIRECTION::POSTIVE_Y);

	m_pTransformCom->Set_Scaling(m_vSize.x, m_vSize.y, 1.f);

	*m_pSpare += 0.00001f;
	//m_pTransformCom->Rotation_Quaternio(XMConvertToRadians(90.f),0.f , 0.f);
	SetUp();
	
	m_fAlpha = 1.f;

	return S_OK;
}

void CEffect_DashRoad::Priority_Tick(_float fTimeDelta)
{

	//__super::Priority_Tick(fTimeDelta);

}

void CEffect_DashRoad::Tick(_float fTimeDelta)
{
	
	__super::Tick(fTimeDelta);
}

void CEffect_DashRoad::Late_Tick(_float fTimeDelta)
{
	if (m_pOwner == nullptr || m_pOwner->Get_Dead())
	{
		Invisibility(fTimeDelta * 5.f);

		m_fTimeAcc += fTimeDelta;

		if(m_fTimeAcc > 0.2f)
			Set_Dead();
	}else{
		if (m_fAlpha > 0.5f)
		{
			Invisibility(fTimeDelta * -5.f);
		}
	}

	__super::Late_Tick(fTimeDelta);

}

HRESULT CEffect_DashRoad::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(6);

	m_pDBufferCom->Bind_Buffer();

	m_pDBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_DashRoad::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_DashRoad::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_POS_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_strTextureTag, &m_pTextureCom))) return E_FAIL;
	if (FAILED(Add_Component<CVIBuffer_DRect>(BUFFER_DRECT_TAG, &m_pDBufferCom))) return E_FAIL;

	return S_OK;
}

void CEffect_DashRoad::SetUp()
{
	CTransform* pOwnerTransform = m_pOwner->Get_Component<CTransform>();

	_vector vTargetUp = XMVector3Normalize(pOwnerTransform->Get_State(CTransform::STATE::STATE_UP));
	_vector vTargetLook = XMVector3Normalize(pOwnerTransform->Get_State(CTransform::STATE::STATE_LOOK));
	_vector vTargetRight = XMVector3Normalize(pOwnerTransform->Get_State(CTransform::STATE::STATE_RIGHT));

	vTargetUp.m128_f32[1] = 0.f;
	vTargetLook.m128_f32[1] = 0.f;
	vTargetRight.m128_f32[1] = 0.f;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_RIGHT, XMVectorSetW(vTargetRight * m_vSize.x, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE::STATE_UP, XMVectorSetW(vTargetLook * m_vSize.y, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE::STATE_LOOK, XMVectorSetW(vTargetUp, 0.f));

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	vPos.m128_f32[1] = 7.1f + *m_pSpare;
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
}

CEffect_DashRoad* CEffect_DashRoad::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_DashRoad* pInstance = new CEffect_DashRoad(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_DashRoad");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_DashRoad::Clone(void* pArg)
{
	CEffect_DashRoad* pInstance = new CEffect_DashRoad(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_DashRoad");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_DashRoad::Free()
{
	__super::Free();

	if(!m_isCloned)
		Safe_Delete(m_pSpare);
}

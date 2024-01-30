#include "stdafx.h"
#include "..\Public\Effect_Trail.h"

#include "GameInstance.h"
#include "Character.h"

CEffect_Trail::CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect(pDevice, pContext)
{
}

CEffect_Trail::CEffect_Trail(const CEffect_Trail& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Trail::Initialize_Prototype()
{
	D3D11_RASTERIZER_DESC;

	return S_OK;
}

HRESULT CEffect_Trail::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_TrainInfo = *(EFFECT_TRAILINFO*)pArg;

	m_vSolid_Color = m_TrainInfo.vSolid_Color;
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Trail::Priority_Tick(_float fTimeDelta)
{
}

void CEffect_Trail::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
}

void CEffect_Trail::Late_Tick(_float fTimeDelta)
{

	//__super::Late_Tick(fTimeDelta);
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLEND, this)))
		return;

	if (m_pOwner == nullptr || m_pOwner->Get_Dead())
		Set_Dead();

}

HRESULT CEffect_Trail::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pTrailCom->Bind_Buffer();
	m_pTrailCom->Render();

	return S_OK;
}

void CEffect_Trail::Trail_Update(_fmatrix matOwner)
{
	m_pTrailCom->Update(matOwner);
}

void CEffect_Trail::Trail_Reset(_fmatrix matOwner)
{
	m_pTrailCom->Reset(matOwner);
}

HRESULT CEffect_Trail::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vSolid_Color", &m_vSolid_Color, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Trail::Ready_Component()
{
	if (FAILED(Add_Component<CShader>(SHADER_POS_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_strTextureTag, &m_pTextureCom))) return E_FAIL;

	CVIBuffer_Trail::VIBUFFER_TRAIL_DESC Trail_Desc = {};
	Trail_Desc.vPos_0 = m_TrainInfo.vTrailPos_0;
	Trail_Desc.vPos_1 = m_TrainInfo.vTrailPos_1;
	Trail_Desc.iMaxCnt = m_TrainInfo.iTrailMaxCnt;
	Trail_Desc.iLerpPointNum = m_TrainInfo.iLerpPointNum;

	if (FAILED(Add_Component<CVIBuffer_Trail>(BUFFER_TRAIL_TAG, &m_pTrailCom, &Trail_Desc))) return E_FAIL;

	return S_OK;
}

CEffect_Trail* CEffect_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Trail* pInstance = new CEffect_Trail(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Trail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Trail::Clone(void* pArg)
{
	CEffect_Trail* pInstance = new CEffect_Trail(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Trail");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Trail::Free()
{
	__super::Free();

	Safe_Release(m_pTrailCom);
}

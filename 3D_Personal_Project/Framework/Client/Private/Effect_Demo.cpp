#include "stdafx.h"
#include "..\Public\Effect_Demo.h"

#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "Bone.h"

CEffect_Demo::CEffect_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CDemo(pDevice, pContext)
{
}

CEffect_Demo::CEffect_Demo(const CEffect_Demo& rhs)
	:CDemo(rhs)
{
}

HRESULT CEffect_Demo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Demo::Initialize(void* pArg)
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

	return S_OK;
}

void CEffect_Demo::Priority_Tick(_float fTimeDelta)
{
	if (m_pSocketBone != nullptr)
	{
		XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() *
			m_pSocketBone->Get_CombinedTransformationMatrix() * m_pParentsTransform->Get_WorldMatrix_Matrix());
		/*_vector vPos = XMVectorSet(m_matWorldMat.m[3][0], m_matWorldMat.m[3][1], m_matWorldMat.m[3][2],1.f);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);*/
	}
}

void CEffect_Demo::Tick(_float fTimeDelta)
{
	if (m_bSprite)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta;

		if (m_fMaxFrame <= m_fFrame)
			m_fFrame = 0.f;
	}

}

void CEffect_Demo::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
		return;
}

HRESULT CEffect_Demo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Demo::Set_Control_Variable(void* pArg)
{
	

	return S_OK;
}

_bool CEffect_Demo::Get_Picked()
{
	return false;
}

void CEffect_Demo::Write_Json(json& Out_Json)
{
	string strTag;

	strTag.assign(m_strModelTag.begin(), m_strModelTag.end());

	Out_Json.emplace("ModelTag", strTag);

	CGameObject::Write_Json(Out_Json);
}

void CEffect_Demo::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("ModelTag") == In_Json.end())
		return;

	string strTag = In_Json["ModelTag"];

	m_strModelTag.assign(strTag.begin(), strTag.end());

	CGameObject::Load_FromJson(In_Json);
}

void CEffect_Demo::Set_SocketBone(CBone* pBone)
{
	if (pBone == nullptr)
		return;

	m_pSocketBone = pBone;
}

HRESULT CEffect_Demo::Bind_ShaderResources()
{
	
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;

	

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	/*if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture")))
		return E_FAIL;*/
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture",m_fFrame)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraPos", &m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS),
		sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraLook", &m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_LOOK),
		sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Demo::Ready_Component()
{

	if (FAILED(Add_Component<CShader>(SHADER_POS_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_strTextureTag, &m_pTextureCom))) return E_FAIL;

	if (FAILED(Add_Component<CVIBuffer_Rect>(BUFFER_PARTICLEPOINT_TAG, &m_pBufferCom))) return E_FAIL;

	return S_OK;
}

CEffect_Demo* CEffect_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Demo* pInstance = new CEffect_Demo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Demo::Clone(void* pArg)
{
	CEffect_Demo* pInstance = new CEffect_Demo(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Demo::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferCom);
}

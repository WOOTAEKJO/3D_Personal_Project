#include "stdafx.h"
#include "../Public/Player/Player_Body.h"

#include "GameInstance.h"

CPlayer_Body::CPlayer_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CPlayer_Body::CPlayer_Body(const CPlayer_Body& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Body::Initialize(void* pArg)
{
	m_pParentsTransform = ((PLAYERBODY_DESC*)pArg)->pParentsTransform;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	//Load_Data("../Bin/Data/Animation/Jack.json");

	if (FAILED(m_pGameInstance->Load_Data_Json(ANIMMODEL_JACK_TAG, this)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Body::Priority_Tick(_float fTimeDelta)
{
	
}

void CPlayer_Body::Tick(_float fTimeDelta)
{
	
}

void CPlayer_Body::Late_Tick(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_matWorldMat ,m_pTransformCom->Get_WorldMatrix_Matrix() * m_pParentsTransform->Get_WorldMatrix_Matrix());

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CPlayer_Body::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(SHADER_TBN::TBN_MODEL);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CPlayer_Body::Write_Json(json& Out_Json)
{
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Write_Json(Out_Json);
}

void CPlayer_Body::Load_FromJson(const json& In_Json)
{
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Load_FromJson(In_Json);
}

HRESULT CPlayer_Body::Load_Data(const _char* strFilePath)
{
	json jLoad;

	if (FAILED(CJson_Utility::Load_Json(strFilePath, jLoad)))
		return E_FAIL;

	Load_FromJson(jLoad);

	return S_OK;
}

HRESULT CPlayer_Body::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorldMat )))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos",
		&m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Body::Ready_Component()
{

	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(ANIMMODEL_JACK_TAG, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

CPlayer_Body* CPlayer_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Body* pInstance = new CPlayer_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Body");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Body::Clone(void* pArg)
{
	CPlayer_Body* pInstance = new CPlayer_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Body");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Body::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

}

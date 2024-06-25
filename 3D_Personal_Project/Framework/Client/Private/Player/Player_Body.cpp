#include "stdafx.h"
#include "../Public/Player/Player_Body.h"

#include "GameInstance.h"

CPlayer_Body::CPlayer_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{// ����̽��� ����̽� ���ؽ�Ʈ�� �޾� �ʱ�ȭ
}

CPlayer_Body::CPlayer_Body(const CPlayer_Body& rhs)
	: CGameObject(rhs)
{// ���� ������
}

HRESULT CPlayer_Body::Initialize_Prototype()
{// ���� ��ü �ʱ�ȭ
	return S_OK;
}

HRESULT CPlayer_Body::Initialize(void* pArg)
{// �纻 ��ü �ʱ�ȭ
	m_pParentsTransform = ((PLAYERBODY_DESC*)pArg)->pParentsTransform;
	// �÷��̾��� Ʈ������ Ŭ������ �޴´�.
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL; // ������Ʈ �غ�

	if (FAILED(m_pGameInstance->Load_Data_Json(ANIMMODEL_JACK_TAG, this)))
		return E_FAIL; // ���̽� ���� �ε�

	return S_OK;
}

void CPlayer_Body::Priority_Tick(_float fTimeDelta)
{// �켱���� ƽ
	XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() * m_pParentsTransform->Get_WorldMatrix_Matrix());
	// ��ü�� ������İ� �÷��̾� ��ü�� ��������� ���ؼ� ������� ������ ����.
	// ��ü�� ��ü�� ������ ����.
}

void CPlayer_Body::Tick(_float fTimeDelta)
{// �Ϲ����� ƽ
	
}

void CPlayer_Body::Late_Tick(_float fTimeDelta)
{// ���� ƽ
	

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_SHADOW, this)))
		return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLUR, this)))
		return;
	// ���� �׷� �߰�
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

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}// ��ü ������

	return S_OK;
}

HRESULT CPlayer_Body::Render_Shadow()
{// ��ü �׸��� ������
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorldMat)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance->Get_ShadowLight()->
		Get_Matrix(CShadowLight::STATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance->Get_ShadowLight()->
		Get_Matrix(CShadowLight::STATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fLightFar", &m_pGameInstance->Get_ShadowLight()->
		Open_Light_Desc()->fFar, sizeof(_float))))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(3);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CPlayer_Body::Render_Blur()
{// Ư�� �޽� �� ������
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		_uint iIndx = 5;
		if (i == 1)
			iIndx = 0;

		m_pShaderCom->Begin(iIndx);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CPlayer_Body::Write_Json(json& Out_Json)
{// ���̽� ���� ����
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Write_Json(Out_Json);
}

void CPlayer_Body::Load_FromJson(const json& In_Json)
{// ���̽� �ε�
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Load_FromJson(In_Json);
}

HRESULT CPlayer_Body::Load_Data(const _char* strFilePath)
{// Ư�� ���� �ּ��� ���̽� ���� �ε�
	json jLoad;

	if (FAILED(CJson_Utility::Load_Json(strFilePath, jLoad)))
		return E_FAIL;

	Load_FromJson(jLoad);

	return S_OK;
}

void CPlayer_Body::Set_AnimationIndex(_uint iAnimIndex)
{// �ִϸ��̼� ��ȯ
	if (m_pModelCom == nullptr)
		return;
	m_pModelCom->Set_AnimationIndex(iAnimIndex);
}

HRESULT CPlayer_Body::Bind_ShaderResources()
{// ���̴� ���ҽ� ���ε�

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_matWorldMat )))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CPlayer_Body::Ready_Component()
{// ������Ʈ �غ�

	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(ANIMMODEL_JACK_TAG, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

CPlayer_Body* CPlayer_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{// ���� ��ü ����
	CPlayer_Body* pInstance = new CPlayer_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Body");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Body::Clone(void* pArg)
{// �纻 ��ü ����
	CPlayer_Body* pInstance = new CPlayer_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Body");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Body::Free()
{// �޸� ����
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

}

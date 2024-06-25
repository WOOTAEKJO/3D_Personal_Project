#include "stdafx.h"
#include "../Public/Player/Player_Body.h"

#include "GameInstance.h"

CPlayer_Body::CPlayer_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{// 디바이스와 디바이스 컨텍스트를 받아 초기화
}

CPlayer_Body::CPlayer_Body(const CPlayer_Body& rhs)
	: CGameObject(rhs)
{// 복사 생성자
}

HRESULT CPlayer_Body::Initialize_Prototype()
{// 원형 객체 초기화
	return S_OK;
}

HRESULT CPlayer_Body::Initialize(void* pArg)
{// 사본 객체 초기화
	m_pParentsTransform = ((PLAYERBODY_DESC*)pArg)->pParentsTransform;
	// 플레이어의 트랜스폼 클래스를 받는다.
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL; // 컴포넌트 준비

	if (FAILED(m_pGameInstance->Load_Data_Json(ANIMMODEL_JACK_TAG, this)))
		return E_FAIL; // 제이슨 파일 로드

	return S_OK;
}

void CPlayer_Body::Priority_Tick(_float fTimeDelta)
{// 우선순위 틱
	XMStoreFloat4x4(&m_matWorldMat, m_pTransformCom->Get_WorldMatrix_Matrix() * m_pParentsTransform->Get_WorldMatrix_Matrix());
	// 몸체의 월드행렬과 플레이어 본체의 월드행렬을 곱해서 월드행렬 변수에 저장.
	// 몸체가 본체를 따르기 위함.
}

void CPlayer_Body::Tick(_float fTimeDelta)
{// 일반적인 틱
	
}

void CPlayer_Body::Late_Tick(_float fTimeDelta)
{// 늦은 틱
	

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_SHADOW, this)))
		return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLUR, this)))
		return;
	// 렌더 그룹 추가
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
	}// 객체 렌더링

	return S_OK;
}

HRESULT CPlayer_Body::Render_Shadow()
{// 객체 그림자 렌더링
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
{// 특정 메쉬 블러 렌더링
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
{// 제이슨 파일 저장
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Write_Json(Out_Json);
}

void CPlayer_Body::Load_FromJson(const json& In_Json)
{// 제이슨 로드
	if (m_pModelCom == nullptr)
		return;

	m_pModelCom->Load_FromJson(In_Json);
}

HRESULT CPlayer_Body::Load_Data(const _char* strFilePath)
{// 특정 파일 주소의 제이슨 파일 로드
	json jLoad;

	if (FAILED(CJson_Utility::Load_Json(strFilePath, jLoad)))
		return E_FAIL;

	Load_FromJson(jLoad);

	return S_OK;
}

void CPlayer_Body::Set_AnimationIndex(_uint iAnimIndex)
{// 애니메이션 전환
	if (m_pModelCom == nullptr)
		return;
	m_pModelCom->Set_AnimationIndex(iAnimIndex);
}

HRESULT CPlayer_Body::Bind_ShaderResources()
{// 셰이더 리소스 바인딩

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
{// 컴포넌트 준비

	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(ANIMMODEL_JACK_TAG, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

CPlayer_Body* CPlayer_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{// 원형 객체 생성
	CPlayer_Body* pInstance = new CPlayer_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Body");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Body::Clone(void* pArg)
{// 사본 객체 생성
	CPlayer_Body* pInstance = new CPlayer_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Body");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Body::Free()
{// 메모리 해제
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

}

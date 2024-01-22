#include "stdafx.h"
#include "..\Public\Particle_Demo.h"

#include "VIBuffer_Particle_Point.h"

CParticle_Demo::CParticle_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CDemo(pDevice, pContext)
{
}

CParticle_Demo::CParticle_Demo(const CParticle_Demo& rhs)
	:CDemo(rhs)
{
}

HRESULT CParticle_Demo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Demo::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	m_eParticleInfo = *(INSTANCING_DESC*)pArg;
	
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Demo::Priority_Tick(_float fTimeDelta)
{
}

void CParticle_Demo::Tick(_float fTimeDelta)
{
	if(m_bUpdate)
		m_pBufferCom->Update(fTimeDelta);
}

void CParticle_Demo::Late_Tick(_float fTimeDelta)
{

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLEND, this)))
		return;
}

HRESULT CParticle_Demo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iIndx = 0;
	if (m_pBufferCom->Open_InstancingDesc()->eColorType == INSTANCING_DESC::COLORTYPE::NORMAL_COLOR)
		iIndx = 0;
	else if(m_pBufferCom->Open_InstancingDesc()->eColorType == INSTANCING_DESC::COLORTYPE::SOLID_COLOR)
		iIndx = 1;

	m_pShaderCom->Begin(iIndx);

	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

	return S_OK;
}

HRESULT CParticle_Demo::Set_Control_Variable(void* pArg)
{
	

	return S_OK;
}

_bool CParticle_Demo::Get_Picked()
{
	return false;
}

HRESULT CParticle_Demo::Save_Particle(const _char* strFilePath)
{
	if (m_pBufferCom == nullptr)
		return E_FAIL;

	return m_pBufferCom->Save_Particle(strFilePath);
}

void CParticle_Demo::Write_Json(json& Out_Json)
{
}

void CParticle_Demo::Load_FromJson(const json& In_Json)
{
}

HRESULT CParticle_Demo::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Texture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraPos", &m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_POS),
		sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCameraLook", &m_pGameInstance->Get_CameraState(CPipeLine::CAMERASTATE::CAM_LOOK),
		sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Demo::Ready_Component()
{

	if (FAILED(Add_Component<CShader>(SHADER_PARTICLEPOINT_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(m_eParticleInfo.strTextureTag, &m_pTextureCom))) return E_FAIL;

	INSTANCING_DESC Instancing_Desc = {};
	Instancing_Desc.vCenter = m_eParticleInfo.vCenter;
	Instancing_Desc.fLifeTime = m_eParticleInfo.fLifeTime;
	Instancing_Desc.fRange = m_eParticleInfo.fRange;

	Instancing_Desc.fScale = m_eParticleInfo.fScale;
	Instancing_Desc.fScaleControl = m_eParticleInfo.fScaleControl;

	Instancing_Desc.fSpeed[0] = m_eParticleInfo.fSpeed[0];
	Instancing_Desc.fSpeed[1] = m_eParticleInfo.fSpeed[1];
	Instancing_Desc.fSpeed[2] = m_eParticleInfo.fSpeed[2];
	Instancing_Desc.fPowerSpeed = m_eParticleInfo.fPowerSpeed;

	Instancing_Desc.iInstanceNum = m_eParticleInfo.iInstanceNum;

	Instancing_Desc.vColor = m_eParticleInfo.vColor;

	Instancing_Desc.vDir = m_eParticleInfo.vDir;
	Instancing_Desc.vRunDir = m_eParticleInfo.vRunDir;


	Instancing_Desc.fRotation[0] = m_eParticleInfo.fRotation[0];
	Instancing_Desc.fRotation[1] = m_eParticleInfo.fRotation[1];
	Instancing_Desc.fRotation[2] = m_eParticleInfo.fRotation[2];

	Instancing_Desc.fRunRotation[0] = m_eParticleInfo.fRunRotation[0];
	Instancing_Desc.fRunRotation[1] = m_eParticleInfo.fRunRotation[1];
	Instancing_Desc.fRunRotation[2] = m_eParticleInfo.fRunRotation[2];

	Instancing_Desc.bLoop = m_eParticleInfo.bLoop;

	Instancing_Desc.eColorType = m_eParticleInfo.eColorType;

	if (FAILED(Add_Component<CVIBuffer_Particle_Point>(BUFFER_PARTICLEPOINT_TAG, &m_pBufferCom,
		&Instancing_Desc))) return E_FAIL;

	return S_OK;
}

CParticle_Demo* CParticle_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Demo* pInstance = new CParticle_Demo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle_Demo::Clone(void* pArg)
{
	CParticle_Demo* pInstance = new CParticle_Demo(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle_Demo::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferCom);
}

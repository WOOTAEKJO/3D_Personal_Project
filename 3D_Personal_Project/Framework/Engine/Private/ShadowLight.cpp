//#include "..\Public\ShadowLight.h"
#include "..\Public\ShadowLight.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

#include "GameInstance.h"

CShadowLight::CShadowLight()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CShadowLight::Initialize(const SHADOW_LIGHT_DESC& eLightDesc)
{
	m_eLight_Desc = eLightDesc;

	return S_OK;
}

void CShadowLight::SetUp_Matrix()
{
	XMStoreFloat4x4(&m_matTransform[STATE::VIEW], XMMatrixLookAtLH(XMLoadFloat4(&m_eLight_Desc.vPos),
		XMLoadFloat4(&m_eLight_Desc.vAt), XMLoadFloat4(&m_eLight_Desc.vUpDir)));

	XMStoreFloat4x4(&m_matTransform[STATE::PROJ], XMMatrixPerspectiveFovLH(m_eLight_Desc.fFov,
		m_eLight_Desc.fAspect, m_eLight_Desc.fNear, m_eLight_Desc.fFar));
}

CShadowLight* CShadowLight::Create(const SHADOW_LIGHT_DESC& eLightDesc)
{
	CShadowLight* pInstance = new CShadowLight();

	if (FAILED(pInstance->Initialize(eLightDesc))) {
		MSG_BOX("Failed to Created : CShadowLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadowLight::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}

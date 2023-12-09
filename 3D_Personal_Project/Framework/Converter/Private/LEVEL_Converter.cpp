#include "stdafx.h"
#include "..\Public\Level_Converter.h"
#include "GameInstance.h"

#include "Level_Loading.h"

CLEVEL_Converter::CLEVEL_Converter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLEVEL_Converter::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLEVEL_Converter::Tick(_float fTimeDelta)
{
	
}

HRESULT CLEVEL_Converter::Render()
{
	SetWindowText(g_hWnd, TEXT("Converter"));

	return S_OK;
}

HRESULT CLEVEL_Converter::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	

	return S_OK;
}

CLEVEL_Converter* CLEVEL_Converter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLEVEL_Converter* pInstance = new CLEVEL_Converter(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLEVEL_Converter");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLEVEL_Converter::Free()
{
	__super::Free();

}

#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "ImGuiMgr.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CMainApp::CMainApp()	
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);

}

HRESULT CMainApp::Initialize()
{	
	GRAPHIC_DESC		GraphicDesc = {};

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.iBackBufferSizeX = g_iWinSizeX;
	GraphicDesc.iBackBufferSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END,GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_ProtoType_Component_ForStaticLevel()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(CImGuiMgr::GetInstance()->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	
	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	/* �׷����� �𵨵��� �׸���.*/	
	m_pGameInstance->Render_Engine();

	if (FAILED(CImGuiMgr::GetInstance()->Render()))
		return E_FAIL;

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eStartLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* ������ �ε��������� ���ۤ��� �Ҳ��� .*/
	CLevel*		pLevel = CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID);
	if (nullptr == pLevel)
		return E_FAIL;

	m_pGameInstance->Open_Level(LEVEL_LOADING,pLevel);

	return S_OK;
}

HRESULT CMainApp::Ready_ProtoType_Component_ForStaticLevel()
{
	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VTXPOSTEX*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXPOSTEX"),
		CShader::Create(m_pDevice,m_pContext,TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxPosTex.hlsli"),VTXPOSTEX::Elements,VTXPOSTEX::iElementsNum))))
		return E_FAIL;

	/* For.Prototype_Component_StateMachine*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_AI*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_STATIC, TEXT("Prototype_Component_AI"),
		CAICom::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	CImGuiMgr::GetInstance()->DestroyInstance();

	/*  �� ����� �����ϸ�. */
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}


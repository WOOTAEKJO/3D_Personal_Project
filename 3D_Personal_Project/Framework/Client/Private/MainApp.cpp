#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "DataMgr.h"

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

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, FILE_PATH, GraphicDesc, g_hInst, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_ProtoType_Component_ForStaticLevel()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(CDataMgr::GetInstance()->Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	
	m_pGameInstance->Tick_Engine(fTimeDelta);

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;

#endif
}

HRESULT CMainApp::Render()
{
	
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	/* �׷����� �𵨵��� �׸���.*/	
	m_pGameInstance->Render_Engine();

	++m_iNumRender;

	if (1.f <= m_fTimeAcc)
	{
		wsprintf(m_szFPS, TEXT("FPS:%d"), m_iNumRender);
		m_iNumRender = 0;
		m_fTimeAcc = 0.f;
	}

	m_pGameInstance->Render_Font(FONT_139EX, m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 0.f, 0.f, 1.f));
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
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Rect>(BUFFER_RECT_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXPOSTEX>(SHADER_POS_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_ETC_ProtoType<CStateMachine>(COM_STATEMACHINE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ETC_ProtoType<CAICom>(COM_AI_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ETC_ProtoType<CRigidBody>(COM_RIGIDBODY_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ETC_ProtoType<CCollider>(COM_COLLIDER_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ETC_ProtoType<CController>(COM_CONTROLLER_TAG))) return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Font()
{
	if (FAILED(m_pGameInstance->Add_Font(FONT_139EX, FONT_139EX_TAG)))
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
	CDataMgr::GetInstance()->DestroyInstance();
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	/*  �� ����� �����ϸ�. */
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}


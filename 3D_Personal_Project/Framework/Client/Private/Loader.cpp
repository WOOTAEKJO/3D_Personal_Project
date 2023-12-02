#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

#include <process.h>

#include "BackGround.h"
#include "Terrain.h"
#include "Mark.h"
#include "DynamicCamera.h"
#include "GameObject_Test.h"
#include "ObjectMesh_Demo.h"

#include "Terrain_Demo.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

// typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);

_uint APIENTRY LoadingThread(void* pArg)
{
	// APIENTRY -> __stdcall

	CoInitializeEx(nullptr, 0);
	// �İ�ü�� �ʱ�ȭ�ϴ� �Լ�
	//	�ٸ� �����忡�� ���� �����尡 ����ϴ� �� ��ü�� ����� �� �� ��ü�� �ʱ�ȭ�ϰ� ����ؾ� �Ѵ�.

	CLoader*		pLoader = (CLoader*)pArg;
	// ���ڷ� ���� Loader ��ü

	pLoader->Loading();
	
	return 0;
}


HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	/* � ������ �ڿ��� �ε��ؾ��ϴ���? */
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);
	// ũ��Ƽ�� ���� �ʱ�ȭ

	/* �����带 �����Ѵ�. */
	/* LoadingThread : ������ �����尡 ���� ���� ȣ���ؾ��� �Լ� */
	/* this : �� �Լ��� ȣ���ϸ鼭 ���ڷ� ������ ������. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThread, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	// this�� ����Լ����� ������ �ִ�.

	return S_OK;
}

void CLoader::Print_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);
	// ������ ȹ���� �䱸

	HRESULT hr = 0;

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;

	case LEVEL_GAMEPLAY:		
		hr = Loading_For_GamePlay_Level();
		break;
	case LEVEL_TOOL:
		hr = Loading_For_Tool_Level();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);
	// ������ �ݳ�

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	/* �ΰ� ������ �ʿ��� �ڿ��� �ε�����. */
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;
		
	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));
	
	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));
	
	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_GameObject_Test"),
		CGameObject_Test::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	/* �����÷��� ������ �ʿ��� �ڿ��� �ε�����. */
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε��ϴ� ���Դϴ�."));

	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Tile%d.dds"),2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain_Mask*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Mask.bmp"), 1))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));
	
	/* For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_DTerrain*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_DTerrain"),
		CVIBuffer_DTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix	matPivot;

	/* For.Prototype_Component_Model_Fiona*/
	matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext,"../Bin/Export/Debug/x64/Resources/Models/Fiona/Fiona.fbx", matPivot))))
		return E_FAIL;

	/* For.Prototype_Component_Model_PineTree*/
	matPivot = XMMatrixScaling(0.01f,0.01f,0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PineTree"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Export/Debug/x64/Resources/Models/PineTree/PineTree.fbx", matPivot))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));

	/* For.Prototype_Component_Shader_VTXNORTEX*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VTXNORTEX"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iElementsNum))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VTXTBN*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VTXTBN"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxTBN.hlsl"), VTXTBN::Elements, VTXTBN::iElementsNum))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));
	
	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DynamicCamera*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_DynamicCamera"), CDynamicCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ObjectMesh_Demo*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_ObjectMesh_Demo"), CObjectMesh_Demo::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tool_Level()
{
	/* �����÷��� ������ �ʿ��� �ڿ��� �ε�����. */
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε��ϴ� ���Դϴ�."));

	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Tile%d.dds"),2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain_Mask*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Terrain_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Mask.bmp"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain_Brush*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Terrain_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));

	/* For.Prototype_Component_VIBuffer_DTerrain*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_DTerrain"),
		CVIBuffer_DTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	_matrix	matPivot;

	/* For.Prototype_Component_Model_PineTree*/
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_Model_PineTree"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Export/Debug/x64/Resources/Models/PineTree/PineTree.fbx", matPivot))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));

	/* For.Prototype_Component_Shader_VTXTBN*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VTXTBN"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxTBN.hlsl"), VTXTBN::Elements, VTXTBN::iElementsNum))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));

	/* For.Prototype_GameObject_Terrain_Demo*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_Terrain_Demo"),
		CTerrain_Demo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DynamicCamera*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_DynamicCamera"), CDynamicCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ObjectMesh_Demo*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_ObjectMesh_Demo"), CObjectMesh_Demo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_GameObject_Mark*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_GameObject_Mark"), CMark::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	// ���� �����尡 �۾� ���̸� ���� ������ ���� ���

	DeleteCriticalSection(&m_CriticalSection);
	// ũ��Ƽ�� ������ ����

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);
	// ������ ����

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

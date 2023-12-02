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
	// 컴객체를 초기화하는 함수
	//	다른 쓰레드에서 메인 쓰레드가 사용하던 컴 객체를 사용할 때 컴 객체를 초기화하고 사용해야 한다.

	CLoader*		pLoader = (CLoader*)pArg;
	// 인자로 받은 Loader 객체

	pLoader->Loading();
	
	return 0;
}


HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	/* 어떤 레벨의 자원을 로드해야하는지? */
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);
	// 크리티컬 섹션 초기화

	/* 스레드를 생성한다. */
	/* LoadingThread : 생성한 스레드가 가장 먼저 호출해야할 함수 */
	/* this : 위 함수를 호출하면서 인자로 전달할 데이터. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThread, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	// this는 멤버함수마다 가지고 있다.

	return S_OK;
}

void CLoader::Print_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);
	// 소유권 획득을 요구

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
	// 소유권 반납

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	/* 로고 레벨에 필요한 자원을 로드하자. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;
		
	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));
	
	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));
	
	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_GameObject_Test"),
		CGameObject_Test::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	/* 게임플레이 레벨에 필요한 자원을 로드하자. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Tile%d.dds"),2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain_Mask*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Mask.bmp"), 1))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));
	
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

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));

	/* For.Prototype_Component_Shader_VTXNORTEX*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VTXNORTEX"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iElementsNum))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VTXTBN*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VTXTBN"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxTBN.hlsl"), VTXTBN::Elements, VTXTBN::iElementsNum))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));
	
	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DynamicCamera*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_DynamicCamera"), CDynamicCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ObjectMesh_Demo*/
	if (FAILED(m_pGameInstance->Add_ProtoType(TEXT("Prototype_GameObject_ObjectMesh_Demo"), CObjectMesh_Demo::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tool_Level()
{
	/* 게임플레이 레벨에 필요한 자원을 로드하자. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

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

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));

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

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));

	/* For.Prototype_Component_Shader_VTXTBN*/
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VTXTBN"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxTBN.hlsl"), VTXTBN::Elements, VTXTBN::iElementsNum))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));

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

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

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
	// 현재 쓰레드가 작업 중이면 끝날 때까지 무한 대기

	DeleteCriticalSection(&m_CriticalSection);
	// 크리티컬 섹션을 삭제

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);
	// 쓰레드 삭제

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

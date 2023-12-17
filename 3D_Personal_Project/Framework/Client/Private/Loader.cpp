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
#include "AnimMesh_Demo.h"

#include "Terrain_Demo.h"

#include "Plateform.h"

/* For.Prototype_Component_Shader_VTXTBN*/
/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VTXTBN"),
	CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxTBN.hlsl"), VTXTBN::Elements, VTXTBN::iElementsNum))))
	return E_FAIL;

#define VTXTBN VTXTBN
#define ELMENTS(type) (type::Elements)
#define ELENENTSNUM(type) (type::iElementsNum)

#define LOAD_SHADER_BTN(eLevel,szKey,szPath)			\
	m_pGameInstance->Add_Component_ProtoType(			\
eLevel,	szKey, 	CShader::Create(m_pDevice, m_pContext,	\	
szPath, ELMENTS(VTXTBN), ELENENTSNUM(VTXTBN)));	*/		

//m_pGameInstance->Add_Shader_ProtoType<VTXTBN>(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VTXTBN"),
//	TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxTBN.hlsl"));

#define GET_SINGLE(type) (type::GetInstance())
#define GAMEINSTANCE GET_SINGLE(CGameInstance)

#define SHADER_PROTOTYPE(type,eLevel,strProtoTag,strFilePaht)		\
	GAMEINSTANCE->Add_Shader_ProtoType<type>(eLevel,strProtoTag,	\
	strFilePaht);

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
	m_pGameInstance->Set_Current_Level(m_eNextLevelID);
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

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_BACKGROUND_TAG, TEX_BACKGROUND_PATH, 2))) return E_FAIL;
		
	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));
	
	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));
	
	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CBackGround>(G0_BACKGROUND_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CGameObject_Test>(GO_GAMEOBJECTTEST_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	/* �����÷��� ������ �ʿ��� �ڿ��� �ε�����. */
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_TAG, TEX_TERRAIN_PATH, 2))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_MASK_TAG, TEX_TERRAIN_MASK_PATH, 1))) return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));
	
	//if (FAILED(m_pGameInstance->Add_Terrain_ProtoType_Height(BUFFER_TERRAIN_TAG, BUFFER_TERRAIN_HEIGHT_PATH))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Terrain_ProtoType_Binary(BUFFER_TERRAIN_TAG, BUFFER_TERRAIN_BIN_PATH))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_DTerrain>(BUFFER_DTERRAIN_TAG))) return E_FAIL;

	_matrix	matPivot;
	matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_FIONA_TAG, ANIMMODEL_FIONA_PATH, matPivot))) return E_FAIL;
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_BAT_TAG, ANIMMODEL_BAT_PATH, matPivot))) return E_FAIL;
	matPivot = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_JACK_TAG, ANIMMODEL_JACK_PATH, matPivot))) return E_FAIL;

#pragma region FOREST
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_PINETREE_TAG, MODEL_PINETREE_PATH, matPivot))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED1_TAG, MODEL_SM_REED1_PATH, matPivot))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED2_TAG, MODEL_SM_REED2_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE2_TAG, MODEL_DEADTREE2_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE3_TAG, MODEL_DEADTREE3_PATH, matPivot))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE4_TAG, MODEL_DEADTREE4_PATH, matPivot))) return E_FAIL;
	
	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FORESTTRUNK_TAG, MODEL_FORESTTRUNK_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE1_TAG, MODEL_SM_CPINE1_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE2_TAG, MODEL_SM_CPINE2_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE3_TAG, MODEL_SM_CPINE3_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_MAPLE1_TAG, MODEL_SM_MAPLE1_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_OWLTREE_TAG, MODEL_SM_OWLTREE_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_PINE1_TAG, MODEL_SM_PINE1_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS_TAG, MODEL_SM_ROOTS_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS1_TAG, MODEL_SM_ROOTS1_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS2_TAG, MODEL_SM_ROOTS2_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS3_TAG, MODEL_SM_ROOTS3_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS4_TAG, MODEL_SM_ROOTS4_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS5_TAG, MODEL_SM_ROOTS5_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TREE_PLATEFORME01X01_TAG, MODEL_TREE_PLATEFORME01X01_PATH, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TRUNKBREAK_TAG, MODEL_TRUNKBREAK_PATH, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODSTOMP_TAG, MODEL_WOODSTOMP_PATH, matPivot))) return E_FAIL;


#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("�׺���̼Ǹ�(��) �ε��ϴ� ���Դϴ�."));
	if (FAILED(m_pGameInstance->Add_Navigation_ProtoType_File(COM_NAVIGATION_TAG, COM_NAVOGATION_PAHT))) return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXNORTEX>(SHADER_NOR_TAG, SHADER_NOR_PATH))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXTBN>(SHADER_BTN_TAG, SHADER_BTN_PATH))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_MESH_TAG, SHADER_MESH_PATH))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXANIMMESH>(SHADER_ANIMMESH_TAG, SHADER_ANIMMESH_PATH))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));
	
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTerrain>(G0_TERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CDynamicCamera>(G0_DCAMERA_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CAnimMesh_Demo>(G0_ANIMMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlateform>(GO_PLATEFORM_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tool_Level()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_TAG, TEX_TERRAIN_PATH, 2))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_MASK_TAG, TEX_TERRAIN_MASK_PATH, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_BRUSH_TAG, TEX_TERRAIN_BRUSH_PATH, 1))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_DTerrain>(BUFFER_DTERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Cube>(BUFFER_CUBE_TAG))) return E_FAIL;

	_matrix	matPivot;

#pragma region FOREST
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_PINETREE_TAG, MODEL_PINETREE_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED1_TAG, MODEL_SM_REED1_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED2_TAG, MODEL_SM_REED2_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE2_TAG, MODEL_DEADTREE2_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE3_TAG, MODEL_DEADTREE3_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE4_TAG, MODEL_DEADTREE4_PATH, matPivot))) return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG, MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FORESTTRUNK_TAG, MODEL_FORESTTRUNK_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE1_TAG, MODEL_SM_CPINE1_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE2_TAG, MODEL_SM_CPINE2_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE3_TAG, MODEL_SM_CPINE3_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_MAPLE1_TAG, MODEL_SM_MAPLE1_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_OWLTREE_TAG, MODEL_SM_OWLTREE_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_PINE1_TAG, MODEL_SM_PINE1_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS_TAG, MODEL_SM_ROOTS_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS1_TAG, MODEL_SM_ROOTS1_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS2_TAG, MODEL_SM_ROOTS2_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS3_TAG, MODEL_SM_ROOTS3_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS4_TAG, MODEL_SM_ROOTS4_PATH, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS5_TAG, MODEL_SM_ROOTS5_PATH, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TREE_PLATEFORME01X01_TAG, MODEL_TREE_PLATEFORME01X01_PATH, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TRUNKBREAK_TAG, MODEL_TRUNKBREAK_PATH, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODSTOMP_TAG, MODEL_WOODSTOMP_PATH, matPivot))) return E_FAIL;


#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("�׺���̼Ǹ�(��) �ε��ϴ� ���Դϴ�."));
	if (FAILED(m_pGameInstance->Add_Navigation_ProtoType_Demo(COM_NAVIGATION_DEMO_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));

	if(FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXTBN>(SHADER_BTN_TAG, SHADER_BTN_PATH))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_MESH_TAG, SHADER_MESH_PATH))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXANIMMESH>(SHADER_ANIMMESH_TAG, SHADER_ANIMMESH_PATH))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTerrain_Demo>(G0_TERRAIN_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CDynamicCamera>(G0_DCAMERA_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CObjectMesh_Demo>(G0_OBJECTMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CAnimMesh_Demo>(G0_ANIMMESH_DEMO_TAG))) return E_FAIL;

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

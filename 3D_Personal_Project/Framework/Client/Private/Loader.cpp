#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

#include <process.h>

#include "BackGround.h"
#include "Terrain.h"
#include "Mark.h"
#include "DynamicCamera.h"
#include "TargetCamera.h"
#include "GameObject_Test.h"

#include "ObjectMesh_Demo.h"
#include "AnimMesh_Demo.h"
#include "SubObject_Demo.h"
#include "Terrain_Demo.h"

#include "Plateform.h"
#include "SkyBox.h"

#include "Player.h"
#include "Player_Body.h"
#include "Player_Weapon_Spear.h"
#include "Player_Weapon_Shovel.h"

#include "Spooketon.h"


CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

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

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_BACKGROUND_TAG, 2))) return E_FAIL;
		
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

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_TAG, 2))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_MASK_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_SKYBOX_TAG, 3))) return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));

	//if (FAILED(m_pGameInstance->Add_Terrain_ProtoType_Height(BUFFER_TERRAIN_TAG, BUFFER_TERRAIN_HEIGHT_PATH))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Terrain_ProtoType_Binary(BUFFER_TERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_DTerrain>(BUFFER_DTERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Cube>(BUFFER_CUBE_TAG))) return E_FAIL;

	_matrix	matPivot;
	matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));

	//matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_BAT_TAG, matPivot))) return E_FAIL;
	matPivot = XMMatrixScaling(0.0005f, 0.0005f, 0.0005f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_JACK_TAG, matPivot))) return E_FAIL;
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_HELICOSCARROW_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_SKULLCROSSBOW_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_SPOOKETON_TAG, matPivot))) return E_FAIL;

#pragma region FOREST

	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_PINETREE_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED2_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE3_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE4_TAG, matPivot))) return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FORESTTRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE3_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_MAPLE1_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_OWLTREE_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_PINE1_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS3_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS4_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS5_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TREE_PLATEFORME01X01_TAG, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TRUNKBREAK_TAG, MODEL_TRUNKBREAK_PATH, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODSTOMP_TAG, MODEL_WOODSTOMP_PATH, matPivot))) return E_FAIL;


#pragma endregion

#pragma region ������

	matPivot = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SPEAR_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SHOVEL_TAG, matPivot))) return E_FAIL;

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("�׺���̼Ǹ�(��) �ε��ϴ� ���Դϴ�."));
	if (FAILED(m_pGameInstance->Add_Navigation_ProtoType_File(COM_NAVIGATION_TAG))) return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXNORTEX>(SHADER_NOR_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_MESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXANIMMESH>(SHADER_ANIMMESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXCUBE>(SHADER_CUBE_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));
	
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTerrain>(G0_TERRAIN_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CDynamicCamera>(G0_DCAMERA_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTargetCamera>(GO_TARGETCAMERA_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CAnimMesh_Demo>(G0_ANIMMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlateform>(GO_PLATEFORM_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CSkyBox>(GO_SKYBOX_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlayer>(ANIMMODEL_JACK_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlayer_Body>(GO_PLAYER_BODY_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlayer_Weapon_Spear>(GO_PLAYER_SPEAR_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlayer_Weapon_Shovel>(GO_PLAYER_SHOVEL_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CSpooketon>(ANIMMODEL_SPOOKETON_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tool_Level()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_TAG, 2))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_MASK_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_BRUSH_TAG, 1))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_DTerrain>(BUFFER_DTERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Cube>(BUFFER_CUBE_TAG))) return E_FAIL;

	_matrix	matPivot;

#pragma region FOREST
	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_PINETREE_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED2_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE3_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE4_TAG, matPivot))) return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FORESTTRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE3_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_MAPLE1_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_OWLTREE_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_PINE1_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS3_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS4_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS5_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TREE_PLATEFORME01X01_TAG, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TRUNKBREAK_TAG, MODEL_TRUNKBREAK_PATH, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODSTOMP_TAG, MODEL_WOODSTOMP_PATH, matPivot))) return E_FAIL;


#pragma endregion

#pragma region ������

	matPivot = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SPEAR_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SHOVEL_TAG, matPivot))) return E_FAIL;

#pragma endregion

	matPivot = XMMatrixScaling(0.001f, 0.001f, 0.001f);
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_JACK_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_HELICOSCARROW_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_SKULLCROSSBOW_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_SPOOKETON_TAG, matPivot))) return E_FAIL;
	

	lstrcpy(m_szLoadingText, TEXT("�׺���̼Ǹ�(��) �ε��ϴ� ���Դϴ�."));
	if (FAILED(m_pGameInstance->Add_Navigation_ProtoType_Demo(COM_NAVIGATION_DEMO_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));

	if(FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXTBN>(SHADER_BTN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_MESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXANIMMESH>(SHADER_ANIMMESH_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CDynamicCamera>(G0_DCAMERA_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTerrain_Demo>(G0_TERRAIN_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CObjectMesh_Demo>(G0_OBJECTMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CAnimMesh_Demo>(G0_ANIMMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CSubObject_Demo>(G0_SUBMESH_DEMO_TAG))) return E_FAIL;

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

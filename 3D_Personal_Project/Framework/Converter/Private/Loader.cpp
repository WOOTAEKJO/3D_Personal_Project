#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

#include <process.h>

#include "Converter_Model.h"
#include "Mesh_Demo.h"

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
	m_pGameInstance->Set_Current_Level(m_eNextLevelID);
	switch (m_eNextLevelID)
	{
	case LEVET_CONVERTER:
		hr = Loading_For_Converter_Level();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);
	// 소유권 반납

	return S_OK;
}

HRESULT CLoader::Loading_For_Converter_Level()
{
	/* 로고 레벨에 필요한 자원을 로드하자. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

		
	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));
	
	/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), ANIMMODEL_JACK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_ANIM, ANIMMODEL_JACK_PATH))))
		return E_FAIL;*/
	
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), ANIMMODEL_CROW_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_ANIM, ANIMMODEL_CROW_PATH))))
		return E_FAIL;
	
#pragma region Level1 Moster

	/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), ANIMMODEL_HELICOSCARROW_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_ANIM, ANIMMODEL_HELICOSCARROW_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), ANIMMODEL_SKULLCROSSBOW_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_ANIM, ANIMMODEL_SKULLCROSSBOW_PATH))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), ANIMMODEL_SPOOKETON_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_ANIM, ANIMMODEL_SPOOKETON_PATH))))
		return E_FAIL;*/


	/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), ANIMMODEL_BAT_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_ANIM, ANIMMODEL_BAT_PATH))))
		return E_FAIL;*/
#pragma endregion

#pragma region FOREST

	/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_REED1_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_REED1_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_REED2_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_REED2_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREE2_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREE2_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREE3_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREE3_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREE4_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREE4_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_FORESTTRUNK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_FORESTTRUNK_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_CPINE1_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_CPINE1_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_CPINE2_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_CPINE2_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_CPINE3_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_CPINE3_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_MAPLE1_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_MAPLE1_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_OWLTREE_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_OWLTREE_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_PINE1_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_PINE1_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_ROOTS_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_ROOTS_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_ROOTS1_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_ROOTS1_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_ROOTS2_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_ROOTS2_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_ROOTS3_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_ROOTS3_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_ROOTS4_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_ROOTS4_PATH))))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SM_ROOTS5_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SM_ROOTS5_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_TREE_PLATEFORME01X01_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_TREE_PLATEFORME01X01_PATH))))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_TRUNKBREAK_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_TRUNKBREAK_PATH))))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_WOODSTOMP_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_WOODSTOMP_PATH))))
		return E_FAIL;*/

#pragma endregion

#pragma region 아이템

	/*if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SPEAR_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SPEAR_PATH))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_SHOVEL_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_SHOVEL_PATH))))
		return E_FAIL;*/

#pragma endregion 

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));
	
	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CMesh_Demo>(G0_MESH_DEMO_TAG))) return E_FAIL;

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

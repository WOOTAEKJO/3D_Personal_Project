#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

#include <process.h>

#include "Converter_Model.h"
#include "Mesh_Demo.h"

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
	
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), ANIMMODEL_FIONA_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_ANIM, ANIMMODEL_FIONA_PATH))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_PINETREE_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_PINETREE_PATH))))
		return E_FAIL;

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

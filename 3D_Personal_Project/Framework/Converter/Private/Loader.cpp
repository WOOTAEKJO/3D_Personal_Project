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
	case LEVET_CONVERTER:
		hr = Loading_For_Converter_Level();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);
	// ������ �ݳ�

	return S_OK;
}

HRESULT CLoader::Loading_For_Converter_Level()
{
	/* �ΰ� ������ �ʿ��� �ڿ��� �ε�����. */
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε��ϴ� ���Դϴ�."));

		
	lstrcpy(m_szLoadingText, TEXT("�𵨸�(��) �ε��ϴ� ���Դϴ�."));
	
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), ANIMMODEL_FIONA_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_ANIM, ANIMMODEL_FIONA_PATH))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_ProtoType(m_pGameInstance->Get_Current_Level(), MODEL_PINETREE_TAG,
		CConverter_Model::Create(m_pDevice, m_pContext, CConverter_Model::TYPE_NONANIM, MODEL_PINETREE_PATH))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε��ϴ� ���Դϴ�."));
	
	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε��ϴ� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CMesh_Demo>(G0_MESH_DEMO_TAG))) return E_FAIL;

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

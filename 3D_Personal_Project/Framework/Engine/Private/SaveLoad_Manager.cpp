#include "..\Public\SaveLoad_Manager.h"
#include "Json/Json_Utility.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>

CSaveLoad_Manager::CSaveLoad_Manager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CSaveLoad_Manager::Initialize()
{
	return S_OK;
}

HRESULT CSaveLoad_Manager::Save_Data_Mesh(const _char* strFileName, CMeshData::MESHDATADESC MeshDataDesc)
{
	CMeshData* pMeshData = CMeshData::Create(MeshDataDesc);
	if (pMeshData == nullptr)
		return E_FAIL;

	if (FAILED(pMeshData->Save_Data(strFileName)))
		return E_FAIL;

	Safe_Release(pMeshData);
	
	return S_OK;
}

HRESULT CSaveLoad_Manager::Load_Data_Mesh(CVIBuffer* pBuffer, const _char* strFileName)
{
	CMeshData::MESHDATADESC MeshDataDesc;

	CMeshData* pMeshData = CMeshData::Create(MeshDataDesc);

	if (FAILED(pMeshData->Load_Data(strFileName)))
		return E_FAIL;

	if (FAILED(pMeshData->Data_Get(MeshDataDesc)))
		return E_FAIL;

	switch (MeshDataDesc.eModel_Type)
	{
	case CMeshData::MODEL_TYPE::ANIM:
		break;
	case CMeshData::MODEL_TYPE::NONANIM:
		break;
	case CMeshData::MODEL_TYPE::TERRAIN:
		
		pBuffer->Init_Buffer(MeshDataDesc);

		break;
	}

	Safe_Release(pMeshData);

	return S_OK;
}

wstring CSaveLoad_Manager::PathFinder(wstring strTag, PAHT_TYPE eType)
{

	wstring strFilePath = TEXT("");

	switch (eType)
	{
	case PAHT_TYPE::TYPE_DATA:
		strFilePath = m_strDataFilePath;
		break;
	case PAHT_TYPE::TYPE_SHADER:
		strFilePath = m_strShaderFilePath;
		break;
	case PAHT_TYPE::TYPE_RESOURCE:
		strFilePath = m_strResourceFilePath;
		break;
	default:
		break;
	}

	wstring strTagName = CUtility_String::BackCut_Wstring(strTag, TEXT('_'));

	for (filesystem::directory_entry entry : filesystem::recursive_directory_iterator(strFilePath))
	{
		wstring strFIleName = entry.path().c_str();

		strFIleName = CUtility_String::Cut_Wstring(strFIleName, TEXT('/'));
		wstring strTmp = CUtility_String::BackCut_Wstring(strFIleName, TEXT('.'));

		if (wcscmp(strTmp.c_str(), strFIleName.c_str()) == 0)
			continue;

		strFIleName = CUtility_String::BackCut_Wstring(strFIleName, TEXT('\\'));

		strFIleName = CUtility_String::Split_Wstring(strFIleName, TEXT('.'));

		if (wcscmp(strTagName.c_str(), strFIleName.c_str()) == 0)
		{
			int a = 0;
			return entry.path().c_str();
		}
	}

	return wstring();
}

void CSaveLoad_Manager::Setting_FilePath(const wstring& strDataPath, const wstring& strShaderPath, const wstring& strResourcePath)
{
	m_strDataFilePath = strDataPath;
	m_strShaderFilePath = strShaderPath;
	m_strResourceFilePath = strResourcePath;
}

CSaveLoad_Manager* CSaveLoad_Manager::Create()
{
	CSaveLoad_Manager* pInstance = new CSaveLoad_Manager();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CSaveLoad_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSaveLoad_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}

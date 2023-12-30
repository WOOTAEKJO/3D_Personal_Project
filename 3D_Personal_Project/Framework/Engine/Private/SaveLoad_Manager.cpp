#include "..\Public\SaveLoad_Manager.h"
#include "Json/Json_Utility.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Level.h"

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

HRESULT CSaveLoad_Manager::Save_Data_Json(const _char* strFilePath, CGameObject* pObject)
{
	json jSave;

	pObject -> Write_Json(jSave);

	if (FAILED(CJson_Utility::Save_Json(strFilePath, jSave)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSaveLoad_Manager::Load_Data_Json(const wstring& strTag, CGameObject* pObject)
{
	json jLoad;
	
	if (FAILED(CJson_Utility::Load_Json(m_pGameInstance->Load_Data_Path(strTag).c_str(), jLoad)))
		return E_FAIL;

	pObject -> Load_FromJson(jLoad);

	return S_OK;
}

HRESULT CSaveLoad_Manager::Save_Data_Level(const _char* strFilePath, _uint iLevelIndex)
{
	

	return S_OK;
}

HRESULT CSaveLoad_Manager::Load_Data_Level(const _char* strFilePath)
{
	

	return S_OK;
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

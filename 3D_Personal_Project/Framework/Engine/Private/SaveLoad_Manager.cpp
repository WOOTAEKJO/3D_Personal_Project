#include "..\Public\SaveLoad_Manager.h"
#include "Json/Json_Utility.h"
#include "MeshData.h"

CSaveLoad_Manager::CSaveLoad_Manager()
{
}

HRESULT CSaveLoad_Manager::Initialize()
{
	return S_OK;
}

HRESULT CSaveLoad_Manager::Save_Data(const _char* strFileName)
{
	

	

	return S_OK;
}

HRESULT CSaveLoad_Manager::Load_Data_Terrain(CVIBuffer_DTerrain* pDTerrain, const _char* strFileName)
{
	CMeshData::MESHDATADESC MeshDataDesc;

	CMeshData* pMeshData = CMeshData::Create(MeshDataDesc);

	if (FAILED(pMeshData->Load_Data(strFileName)))
		return E_FAIL;

	if (FAILED(pMeshData->Data_Get(MeshDataDesc)))
		return E_FAIL;

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
}

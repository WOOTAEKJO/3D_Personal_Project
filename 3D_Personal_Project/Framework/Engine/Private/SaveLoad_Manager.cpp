#include "..\Public\SaveLoad_Manager.h"
#include "Json/Json_Utility.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MeshData.h"

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

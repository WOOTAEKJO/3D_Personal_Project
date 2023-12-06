#include "..\Public\MeshData.h"
#include "Json/Json_Utility.h"

CMeshData::CMeshData()
{
}

HRESULT CMeshData::Initialize(MESHDATADESC MeshDataDesc)
{
	if (MeshDataDesc.eModel_Type != TYPE_END) {

		if (FAILED(Set_Data(MeshDataDesc)))
			return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CMeshData::Save_Data(const char* strPath)
{
	ofstream fout;

	fout.open(strPath,std::ofstream::binary);

	if (fout.is_open())
	{
		fout.write(reinterpret_cast<const char*>(&m_eModel_Type), sizeof(m_eModel_Type));
		fout.write(reinterpret_cast<const char*>(&m_iNumVertices), sizeof(m_iNumVertices));
		fout.write(reinterpret_cast<const char*>(&m_iNumFaces), sizeof(m_iNumFaces));
		fout.write(reinterpret_cast<const char*>(&m_iNumBones), sizeof(m_iNumBones));

		switch (m_eModel_Type)
		{
		case Engine::CMeshData::NONANIM:
			break;
		case Engine::CMeshData::ANIM:
			break;
		case Engine::CMeshData::TERRAIN:

			for (_uint i =0;i< m_iNumVertices;i++)
			{
				fout.write(reinterpret_cast<char*>(&m_vecMeshVertices[i]), sizeof(VTXMESH));
			}

			break;
		}

		string line;

		for (_uint i = 0; i < m_iNumFaces; i++)
		{
			fout.write(reinterpret_cast<char*>(&m_vecIndices[i]), sizeof(_uint3));
		}

	}
	else
		return E_FAIL;


	fout.close();
	
	return S_OK;
}

HRESULT CMeshData::Load_Data(const char* strPath)
{
	ifstream fIn;

	fIn.open(strPath, std::ifstream::binary);

	if (fIn.is_open())
	{
		fIn.read(reinterpret_cast<char*>(&m_eModel_Type), sizeof(m_eModel_Type));
		fIn.read(reinterpret_cast<char*>(&m_iNumVertices), sizeof(m_iNumVertices));
		fIn.read(reinterpret_cast<char*>(&m_iNumFaces), sizeof(m_iNumFaces));
		fIn.read(reinterpret_cast<char*>(&m_iNumBones), sizeof(m_iNumBones));

		switch (m_eModel_Type)
		{
		case Engine::CMeshData::NONANIM:
			break;
		case Engine::CMeshData::ANIM:
			break;
		case Engine::CMeshData::TERRAIN:

			VTXMESH* tTerrain = new VTXMESH[m_iNumVertices];

			for (_uint i = 0; i < m_iNumVertices; i++)
			{
				
				fIn.read(reinterpret_cast<char*>(&tTerrain[i]), sizeof(VTXMESH));
				m_vecMeshVertices.push_back(tTerrain[i]);
				//memcpy(&m_MeshVertices[i], &tTerrain[i], sizeof(VTXMESH));
				//fIn >> (&tTerrain[i]);
			}
			break;
		}

		_uint3* iIndex = new _uint3[m_iNumFaces];

		for (_uint i = 0; i < m_iNumFaces; i++)
		{

			fIn.read(reinterpret_cast<char*>(&iIndex[i]), sizeof(_uint3));
			
			m_vecIndices.push_back(iIndex[i]);
			//memcpy(&m_Indices[i], &iIndex[i], sizeof(_uint3));
		}
	}
	else
		return E_FAIL;

	fIn.close();

	return S_OK;
}

HRESULT CMeshData::Data_Get(MESHDATADESC& MeshDataDesc)
{
	MeshDataDesc.eModel_Type = m_eModel_Type;
	MeshDataDesc.iNumVertices = m_iNumVertices;
	MeshDataDesc.iNumFaces = m_iNumFaces;
	MeshDataDesc.iNumBones = m_iNumBones;

	switch (m_eModel_Type)
	{
	case Engine::CMeshData::NONANIM:
		 MeshDataDesc.vecMeshVertices = m_vecMeshVertices;
		break;
	case Engine::CMeshData::ANIM:
		 MeshDataDesc.vecAnimVertices = m_vecAnimVertices;
		break;
	case Engine::CMeshData::TERRAIN:
		 MeshDataDesc.vecMeshVertices = m_vecMeshVertices;
		break;
	}

	MeshDataDesc.vecIndices = m_vecIndices;

	return S_OK;
}

HRESULT CMeshData::Set_Data(MESHDATADESC MeshDataDesc)
{
	m_eModel_Type = MeshDataDesc.eModel_Type;
	m_iNumVertices = MeshDataDesc.iNumVertices;
	m_iNumFaces = MeshDataDesc.iNumFaces;
	m_iNumBones = MeshDataDesc.iNumBones;

	switch (m_eModel_Type)
	{
	case Engine::CMeshData::NONANIM:
		m_vecMeshVertices = MeshDataDesc.vecMeshVertices;
		break;
	case Engine::CMeshData::ANIM:
		m_vecAnimVertices = MeshDataDesc.vecAnimVertices;
		break;
	case Engine::CMeshData::TERRAIN:
		m_vecMeshVertices = MeshDataDesc.vecMeshVertices;
		break;
	}

	m_vecIndices = MeshDataDesc.vecIndices;

	return S_OK;
}

CMeshData* CMeshData::Create(MESHDATADESC MeshDataDesc)
{
	CMeshData* pInstance = new CMeshData();

	if (FAILED(pInstance->Initialize(MeshDataDesc))) {
		MSG_BOX("Failed to Created : CMeshData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshData::Free()
{
	__super::Free();
}

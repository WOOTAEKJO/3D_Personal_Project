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
		

		switch (m_eModel_Type)
		{
		case Engine::CMeshData::NONANIM:
			break;
		case Engine::CMeshData::ANIM:

			fout.write(reinterpret_cast<const char*>(&m_iMeshNum), sizeof(m_iMeshNum));
			fout.write(reinterpret_cast<const char*>(&m_iMaterialNum), sizeof(m_iMaterialNum));
			fout.write(reinterpret_cast<const char*>(&m_iAnimBoneNum), sizeof(m_iAnimBoneNum));
			fout.write(reinterpret_cast<const char*>(&m_iAnimAnimationNum), sizeof(m_iAnimAnimationNum));

			for (_uint i = 0; i < m_iMeshNum; i++)
				fout.write(reinterpret_cast<char*>(&m_vecMesh[i]), sizeof(MESH));

			for (_uint i = 0; i < m_iMaterialNum; i++)
				fout.write(reinterpret_cast<char*>(&m_vecMaterial[i]), sizeof(MESH));

			for (_uint i = 0; i < m_iAnimBoneNum; i++)
				fout.write(reinterpret_cast<char*>(&m_vecAnimBone[i]), sizeof(MESH));

			for (_uint i = 0; i < m_iAnimAnimationNum; i++)
				fout.write(reinterpret_cast<char*>(&m_vecAnimAnimation[i]), sizeof(MESH));
			
			break;
		case Engine::CMeshData::TERRAIN:

			fout.write(reinterpret_cast<const char*>(&m_eModel_Type), sizeof(m_eModel_Type));
			fout.write(reinterpret_cast<const char*>(&m_iNumVertices), sizeof(m_iNumVertices));
			fout.write(reinterpret_cast<const char*>(&m_iNumFaces), sizeof(m_iNumFaces));

			for (_uint i =0;i< m_iNumVertices;i++)
			{
				fout.write(reinterpret_cast<char*>(&m_vecMeshVertices[i]), sizeof(VTXMESH));
			}

			for (_uint i = 0; i < m_iNumFaces; i++)
			{
				fout.write(reinterpret_cast<char*>(&m_vecIndices[i]), sizeof(_uint3));
			}

			break;
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

		switch (m_eModel_Type)
		{
		case Engine::CMeshData::NONANIM:
			break;
		case Engine::CMeshData::ANIM:
			



			break;


			break;
		case Engine::CMeshData::TERRAIN:

			//VTXMESH* tTerrain = new VTXMESH[m_iNumVertices];

			for (_uint i = 0; i < m_iNumVertices; i++)
			{
				VTXMESH tTerrain = {};

				fIn.read(reinterpret_cast<char*>(&tTerrain), sizeof(VTXMESH));
				m_vecMeshVertices.push_back(tTerrain);
				//memcpy(&m_MeshVertices[i], &tTerrain[i], sizeof(VTXMESH));
				//fIn >> (&tTerrain[i]);
			}
			break;
		}

		//_uint3* iIndex = new _uint3[m_iNumFaces];

		for (_uint i = 0; i < m_iNumFaces; i++)
		{
			_uint3 iIndex = {};
			fIn.read(reinterpret_cast<char*>(&iIndex), sizeof(_uint3));
			
			m_vecIndices.push_back(iIndex);
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

	switch (m_eModel_Type)
	{
	case Engine::CMeshData::NONANIM:
		 MeshDataDesc.vecMeshVertices = m_vecMeshVertices;
		break;
	case Engine::CMeshData::ANIM:
		 
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

	switch (m_eModel_Type)
	{
	case Engine::CMeshData::NONANIM:
		m_vecMeshVertices = MeshDataDesc.vecMeshVertices;
		break;
	case Engine::CMeshData::ANIM:
		/*m_vecAnimMesh = MeshDataDesc.vecAnimMesh;
		m_vecAnimBone = MeshDataDesc.vecAnimBone;
		m_vecAnimMaterial = MeshDataDesc.vecAnimMaterial;
		m_vecAnimAnimation = MeshDataDesc.vecAnimAnimation;
		*/
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

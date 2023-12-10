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

		switch (m_eModel_Type)
		{
		case Engine::CMeshData::NONANIM:
			fout.write(reinterpret_cast<const char*>(&m_iMeshNum), sizeof(m_iMeshNum));
			fout.write(reinterpret_cast<const char*>(&m_iMaterialNum), sizeof(m_iMaterialNum));
			fout.write(reinterpret_cast<const char*>(&m_iAnimBoneNum), sizeof(m_iAnimBoneNum));

			for (_uint i = 0; i < m_iMeshNum; i++) {

				size_t nameSize = m_vecMesh[i].szName.size();
				fout.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
				fout.write(m_vecMesh[i].szName.c_str(), nameSize);

				fout.write(reinterpret_cast<const char*>(&m_vecMesh[i].iMaterialIndex), sizeof(_uint));

				_uint iSize = m_vecMesh[i].vecMeshBoneIndices.size();
				fout.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));
				fout.write(reinterpret_cast<const char*>(m_vecMesh[i].vecMeshBoneIndices.data()), iSize * sizeof(_uint));

				iSize = m_vecMesh[i].vecVerticesNonAim.size();
				fout.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));
				fout.write(reinterpret_cast<const char*>(m_vecMesh[i].vecVerticesNonAim.data()), iSize * sizeof(VTXMESH));

				iSize = m_vecMesh[i].vecIndices.size();
				fout.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));
				fout.write(reinterpret_cast<const char*>(m_vecMesh[i].vecIndices.data()), iSize * sizeof(_uint3));
			}

			for (_uint i = 0; i < m_iMaterialNum; i++) {
				for (_uint j = 0; j < 18; j++)
				{
					size_t nameSize = m_vecMaterial[i].vecMaterialPath[j].size();
					fout.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
					fout.write(m_vecMaterial[i].vecMaterialPath[j].c_str(), nameSize);
				}
			}


			for (_uint i = 0; i < m_iAnimBoneNum; i++) {
				size_t nameSize = m_vecAnimBone[i].szName.size();
				fout.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
				fout.write(m_vecAnimBone[i].szName.c_str(), nameSize);

				fout.write(reinterpret_cast<const char*>(&m_vecAnimBone[i].iParentIndex), sizeof(_int));
				fout.write(reinterpret_cast<const char*>(&m_vecAnimBone[i].matTransformation), sizeof(_float4x4));
				fout.write(reinterpret_cast<const char*>(&m_vecAnimBone[i].matOffsetMatrix), sizeof(_float4x4));
			}
			break;
		case Engine::CMeshData::ANIM:

			fout.write(reinterpret_cast<const char*>(&m_iMeshNum), sizeof(m_iMeshNum));
			fout.write(reinterpret_cast<const char*>(&m_iMaterialNum), sizeof(m_iMaterialNum));
			fout.write(reinterpret_cast<const char*>(&m_iAnimBoneNum), sizeof(m_iAnimBoneNum));
			fout.write(reinterpret_cast<const char*>(&m_iAnimAnimationNum), sizeof(m_iAnimAnimationNum));

			for (_uint i = 0; i < m_iMeshNum; i++) {

				size_t nameSize = m_vecMesh[i].szName.size();
				fout.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
				fout.write(m_vecMesh[i].szName.c_str(), nameSize);

				fout.write(reinterpret_cast<const char*>(&m_vecMesh[i].iMaterialIndex), sizeof(_uint));

				_uint iSize = m_vecMesh[i].vecMeshBoneIndices.size();
				fout.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));
				fout.write(reinterpret_cast<const char*>(m_vecMesh[i].vecMeshBoneIndices.data()), iSize * sizeof(_uint));

				iSize = m_vecMesh[i].vecVerticesAnim.size();
				fout.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));
				fout.write(reinterpret_cast<const char*>(m_vecMesh[i].vecVerticesAnim.data()), iSize*sizeof(VTXANIMMESH));

				iSize = m_vecMesh[i].vecIndices.size();
				fout.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));
				fout.write(reinterpret_cast<const char*>(m_vecMesh[i].vecIndices.data()), iSize * sizeof(_uint3));
			}

			for (_uint i = 0; i < m_iMaterialNum; i++) {
				for (_uint j = 0; j < 18; j++)
				{
					size_t nameSize = m_vecMaterial[i].vecMaterialPath[j].size();
					fout.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
					fout.write(m_vecMaterial[i].vecMaterialPath[j].c_str(), nameSize);
				}
			}
				

			for (_uint i = 0; i < m_iAnimBoneNum; i++) {
				size_t nameSize = m_vecAnimBone[i].szName.size();
				fout.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
				fout.write(m_vecAnimBone[i].szName.c_str(), nameSize);

				fout.write(reinterpret_cast<const char*>(&m_vecAnimBone[i].iParentIndex), sizeof(_int));
				fout.write(reinterpret_cast<const char*>(&m_vecAnimBone[i].matTransformation), sizeof(_float4x4));
				fout.write(reinterpret_cast<const char*>(&m_vecAnimBone[i].matOffsetMatrix), sizeof(_float4x4));
			}
				

			for (_uint i = 0; i < m_iAnimAnimationNum; i++) {
			
				size_t nameSize = m_vecAnimAnimation[i].szName.size();
				fout.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
				fout.write(m_vecAnimAnimation[i].szName.c_str(), nameSize);
				
				fout.write(reinterpret_cast<const char*>(&m_vecAnimAnimation[i].fDuration), sizeof(_float));
				fout.write(reinterpret_cast<const char*>(&m_vecAnimAnimation[i].fTicksPerSecond), sizeof(_float));

				_uint iSize = m_vecAnimAnimation[i].vecChannel.size();
				fout.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));

				for (_uint j = 0; j < iSize; j++)
				{
					size_t cnameSize = m_vecAnimAnimation[i].vecChannel[j].szName.size();
					fout.write(reinterpret_cast<const char*>(&cnameSize), sizeof(size_t));
					fout.write(m_vecAnimAnimation[i].vecChannel[j].szName.c_str(), cnameSize);

					_uint icSize = m_vecAnimAnimation[i].vecChannel[j].vecKeyFrame.size();
					fout.write(reinterpret_cast<const char*>(&icSize), sizeof(_uint));
					fout.write(reinterpret_cast<const char*>(m_vecAnimAnimation[i].vecChannel[j].vecKeyFrame.data()), icSize * sizeof(KEYFRAME));
				}
			}
				
			break;
		case Engine::CMeshData::TERRAIN:

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

	fIn.open(strPath, std::ios::binary);

	if (fIn.is_open())
	{
		fIn.read(reinterpret_cast<char*>(&m_eModel_Type), sizeof(m_eModel_Type));
		

		switch (m_eModel_Type)
		{
		case Engine::CMeshData::NONANIM:
			fIn.read(reinterpret_cast<char*>(&m_iMeshNum), sizeof(m_iMeshNum));
			fIn.read(reinterpret_cast<char*>(&m_iMaterialNum), sizeof(m_iMaterialNum));
			fIn.read(reinterpret_cast<char*>(&m_iAnimBoneNum), sizeof(m_iAnimBoneNum));

			for (_uint i = 0; i < m_iMeshNum; i++) {

				MESH	Mesh = {};

				size_t	nameSize = {};
				fIn.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
				Mesh.szName.resize(nameSize);
				fIn.read(&Mesh.szName[0], nameSize);

				fIn.read(reinterpret_cast<char*>(&Mesh.iMaterialIndex), sizeof(_uint));

				_uint iSize = {};

				fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
				Mesh.vecMeshBoneIndices.resize(iSize);
				fIn.read(reinterpret_cast<char*>(Mesh.vecMeshBoneIndices.data()), iSize * sizeof(_uint));

				fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
				Mesh.vecVerticesNonAim.resize(iSize);
				fIn.read(reinterpret_cast<char*>(Mesh.vecVerticesNonAim.data()), iSize * sizeof(VTXMESH));

				fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
				Mesh.vecIndices.resize(iSize);
				fIn.read(reinterpret_cast<char*>(Mesh.vecIndices.data()), iSize * sizeof(_uint3));

				m_vecMesh.push_back(Mesh);
			}


			for (_uint i = 0; i < m_iMaterialNum; i++) {

				MATERIAL Material = {};

				for (_uint j = 0; j < 18; j++)
				{
					size_t nameSize = {};
					fIn.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
					Material.vecMaterialPath[j].resize(nameSize);
					fIn.read(&Material.vecMaterialPath[j][0], nameSize);
				}

				m_vecMaterial.push_back(Material);
			}


			for (_uint i = 0; i < m_iAnimBoneNum; i++) {

				BONE	Bone = {};

				size_t nameSize = {};
				fIn.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
				Bone.szName.resize(nameSize);
				fIn.read(&Bone.szName[0], nameSize);

				fIn.read(reinterpret_cast<char*>(&Bone.iParentIndex), sizeof(_int));
				fIn.read(reinterpret_cast<char*>(&Bone.matTransformation), sizeof(_float4x4));
				fIn.read(reinterpret_cast<char*>(&Bone.matOffsetMatrix), sizeof(_float4x4));

				m_vecAnimBone.push_back(Bone);
			}
			break;
		case Engine::CMeshData::ANIM:

			fIn.read(reinterpret_cast<char*>(&m_iMeshNum), sizeof(m_iMeshNum));
			fIn.read(reinterpret_cast<char*>(&m_iMaterialNum), sizeof(m_iMaterialNum));
			fIn.read(reinterpret_cast<char*>(&m_iAnimBoneNum), sizeof(m_iAnimBoneNum));
			fIn.read(reinterpret_cast<char*>(&m_iAnimAnimationNum), sizeof(m_iAnimAnimationNum));

			for (_uint i = 0; i < m_iMeshNum; i++){

				MESH	Mesh = {};

				size_t	nameSize = {};
				fIn.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
				Mesh.szName.resize(nameSize);
				fIn.read(&Mesh.szName[0], nameSize);

				fIn.read(reinterpret_cast<char*>(&Mesh.iMaterialIndex), sizeof(_uint));

				_uint iSize = {};

				fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
				Mesh.vecMeshBoneIndices.resize(iSize);
				fIn.read(reinterpret_cast<char*>(Mesh.vecMeshBoneIndices.data()), iSize * sizeof(_uint));

				fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
				Mesh.vecVerticesAnim.resize(iSize);
				fIn.read(reinterpret_cast<char*>(Mesh.vecVerticesAnim.data()), iSize *sizeof(VTXANIMMESH));

				fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
				Mesh.vecIndices.resize(iSize);
				fIn.read(reinterpret_cast<char*>(Mesh.vecIndices.data()), iSize * sizeof(_uint3));

				m_vecMesh.push_back(Mesh);
			}
				

			for (_uint i = 0; i < m_iMaterialNum; i++) {
				
				MATERIAL Material = {};

				for (_uint j = 0; j < 18; j++)
				{
					size_t nameSize = {};
					fIn.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
					Material.vecMaterialPath[j].resize(nameSize);
					fIn.read(&Material.vecMaterialPath[j][0], nameSize);
				}

				m_vecMaterial.push_back(Material);
			}
				

			for (_uint i = 0; i < m_iAnimBoneNum; i++) {

				BONE	Bone = {};
				
				size_t nameSize = {};
				fIn.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
				Bone.szName.resize(nameSize);
				fIn.read(&Bone.szName[0], nameSize);

				fIn.read(reinterpret_cast<char*>(&Bone.iParentIndex), sizeof(_int));
				fIn.read(reinterpret_cast<char*>(&Bone.matTransformation), sizeof(_float4x4));
				fIn.read(reinterpret_cast<char*>(&Bone.matOffsetMatrix), sizeof(_float4x4));

				m_vecAnimBone.push_back(Bone);
			}
				

			for (_uint i = 0; i < m_iAnimAnimationNum; i++) {

				ANIMATION Animation = {};

				size_t nameSize = {};
				fIn.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
				Animation.szName.resize(nameSize);
				fIn.read(&Animation.szName[0], nameSize);

				fIn.read(reinterpret_cast<char*>(&Animation.fDuration), sizeof(_float));
				fIn.read(reinterpret_cast<char*>(&Animation.fTicksPerSecond), sizeof(_float));

				_uint iSize = {};
				fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));

				//Animation.vecChannel.resize(iSize);
				for (_uint j = 0; j < iSize; j++)
				{
					CHANNEL Channel;

					size_t cnameSize = {};
					fIn.read(reinterpret_cast<char*>(&cnameSize), sizeof(size_t));
					Channel.szName.resize(cnameSize);
					fIn.read(&Channel.szName[0], cnameSize);

					_uint icSize = {};
					fIn.read(reinterpret_cast<char*>(&icSize), sizeof(_uint));
					Channel.vecKeyFrame.resize(icSize);
					fIn.read(reinterpret_cast<char*>(Channel.vecKeyFrame.data()), icSize * sizeof(KEYFRAME));

					Animation.vecChannel.push_back(Channel);
				}

				m_vecAnimAnimation.push_back(Animation);
			}
			
			break;
		case Engine::CMeshData::TERRAIN:

			fIn.read(reinterpret_cast<char*>(&m_iNumVertices), sizeof(m_iNumVertices));
			fIn.read(reinterpret_cast<char*>(&m_iNumFaces), sizeof(m_iNumFaces));

			for (_uint i = 0; i < m_iNumVertices; i++)
			{
				VTXMESH tTerrain = {};

				fIn.read(reinterpret_cast<char*>(&tTerrain), sizeof(VTXMESH));
				m_vecMeshVertices.push_back(tTerrain);
			}

			for (_uint i = 0; i < m_iNumFaces; i++)
			{
				_uint3 iIndex = {};
				fIn.read(reinterpret_cast<char*>(&iIndex), sizeof(_uint3));

				m_vecIndices.push_back(iIndex);
			}
			break;
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
	
	switch (m_eModel_Type)
	{
	case Engine::CMeshData::NONANIM:
		MeshDataDesc.iMeshNum = m_iMeshNum;
		MeshDataDesc.iMaterialNum = m_iMaterialNum;
		MeshDataDesc.iAnimBoneNum = m_iAnimBoneNum;

		MeshDataDesc.vecMesh = m_vecMesh;
		MeshDataDesc.vecMaterial = m_vecMaterial;
		MeshDataDesc.vecAnimBone = m_vecAnimBone;
		break;
	case Engine::CMeshData::ANIM:
		MeshDataDesc.iMeshNum = m_iMeshNum;
		MeshDataDesc.iMaterialNum = m_iMaterialNum;
		MeshDataDesc.iAnimBoneNum = m_iAnimBoneNum;
		MeshDataDesc.iAnimAnimationNum = m_iAnimAnimationNum;

		MeshDataDesc.vecMesh = m_vecMesh;
		MeshDataDesc.vecMaterial = m_vecMaterial;
		MeshDataDesc.vecAnimBone = m_vecAnimBone;
		MeshDataDesc.vecAnimAnimation = m_vecAnimAnimation;
		 
		break;
	case Engine::CMeshData::TERRAIN:
		MeshDataDesc.iNumVertices = m_iNumVertices;
		MeshDataDesc.iNumFaces = m_iNumFaces;
		MeshDataDesc.vecMeshVertices = m_vecMeshVertices;
		MeshDataDesc.vecIndices = m_vecIndices;
		break;
	}

	return S_OK;
}

HRESULT CMeshData::Set_Data(MESHDATADESC MeshDataDesc)
{
	m_eModel_Type = MeshDataDesc.eModel_Type;
	

	switch (m_eModel_Type)
	{
	case Engine::CMeshData::NONANIM:
		m_iMeshNum = MeshDataDesc.iMeshNum;
		m_iMaterialNum = MeshDataDesc.iMaterialNum;
		m_iAnimBoneNum = MeshDataDesc.iAnimBoneNum;

		m_vecMesh = MeshDataDesc.vecMesh;
		m_vecAnimBone = MeshDataDesc.vecAnimBone;
		m_vecMaterial = MeshDataDesc.vecMaterial;
		break;
	case Engine::CMeshData::ANIM:

		m_iMeshNum = MeshDataDesc.iMeshNum;
		m_iMaterialNum = MeshDataDesc.iMaterialNum;
		m_iAnimBoneNum = MeshDataDesc.iAnimBoneNum;
		m_iAnimAnimationNum = MeshDataDesc.iAnimAnimationNum;

		m_vecMesh = MeshDataDesc.vecMesh;
		m_vecAnimBone = MeshDataDesc.vecAnimBone;
		m_vecMaterial = MeshDataDesc.vecMaterial;
		m_vecAnimAnimation = MeshDataDesc.vecAnimAnimation;
		
		break;
	case Engine::CMeshData::TERRAIN:
		m_iNumVertices = MeshDataDesc.iNumVertices;
		m_iNumFaces = MeshDataDesc.iNumFaces;
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

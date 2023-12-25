#include "stdafx.h"
#include "..\Public\Converter_Model.h"

#include "GameInstance.h"

CConverter_Model::CConverter_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CConverter_Model::CConverter_Model(const CConverter_Model& rhs)
	:CVIBuffer(rhs),
	m_eType(rhs.m_eType),
	m_bReady(rhs.m_bReady), m_iMeshesNum(rhs.m_iMeshesNum),m_vecMesh(rhs.m_vecMesh),
	m_iMaterialsNum(rhs.m_iMaterialsNum),m_vecMaterial(rhs.m_vecMaterial),
	m_iBonesNum(rhs.m_iBonesNum),m_vecBone(rhs.m_vecBone),
	m_iAnimationsNum(rhs.m_iAnimationsNum),m_vecAnimation(rhs.m_vecAnimation)
{
}

HRESULT CConverter_Model::Initialize_ProtoType(TYPE eType, const string& strModelFilePath)
{
	m_eType = eType;

	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (m_eType == TYPE::TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAiScene = m_Importer.ReadFile(strModelFilePath, iFlag);
	// aiProcess_PreTransformVertices 애니메이션이 있는 매쉬를 로드하면 애니메이션과 관련된 매쉬가 없어질 수 있다.
	if (m_pAiScene == nullptr)
		return E_FAIL;

	if (FAILED(Ready_Bones(m_pAiScene->mRootNode, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(strModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_vecMesh;
	m_vecBone;
	m_vecMaterial;
	m_vecAnimation;

	m_bReady = true;

	return S_OK;
}

HRESULT CConverter_Model::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CConverter_Model::Set_Buffer(const _char* strPath)
{
	if (!m_bReady)
		return E_FAIL;
	
	CMeshData::MESHDATADESC MeshDataDesc;

	if (m_eType == TYPE::TYPE_ANIM) {
		MeshDataDesc.eModel_Type = CMeshData::MODEL_TYPE::ANIM;
		MeshDataDesc.iMeshNum = m_iMeshesNum;
		MeshDataDesc.iMaterialNum = m_iMaterialsNum;
		MeshDataDesc.iAnimBoneNum = m_iBonesNum;
		MeshDataDesc.iAnimAnimationNum = m_iAnimationsNum;

		MeshDataDesc.vecMesh = m_vecMesh;
		MeshDataDesc.vecMaterial = m_vecMaterial;
		MeshDataDesc.vecAnimBone = m_vecBone;
		MeshDataDesc.vecAnimAnimation = m_vecAnimation;
		
	}
	else if (m_eType == TYPE::TYPE_NONANIM) {
		MeshDataDesc.eModel_Type = CMeshData::MODEL_TYPE::NONANIM;
		MeshDataDesc.iMeshNum = m_iMeshesNum;
		MeshDataDesc.iMaterialNum = m_iMaterialsNum;
		MeshDataDesc.iAnimBoneNum = m_iBonesNum;

		MeshDataDesc.vecMesh = m_vecMesh;
		MeshDataDesc.vecMaterial = m_vecMaterial;
		MeshDataDesc.vecAnimBone = m_vecBone;
	}
	else
		return E_FAIL;
	

	if (FAILED(m_pGameInstance->Save_Data_Mesh(strPath, MeshDataDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CConverter_Model::Ready_Meshes()
{
	m_iBonesNum = m_vecBone.size();

	m_iMeshesNum = m_pAiScene->mNumMeshes;
	m_vecMesh.reserve(m_iMeshesNum);

	for (_uint i = 0; i < m_iMeshesNum; i++)
	{
		MESH Mesh = {};
		
		aiMesh* pMesh = m_pAiScene->mMeshes[i];

		//strcpy_s(Mesh.szName, pMesh->mName.data);
		Mesh.szName = pMesh->mName.data;

		Mesh.iMaterialIndex = pMesh->mMaterialIndex;

		if (m_eType == TYPE::TYPE_ANIM)
		{
			
			Mesh.vecVerticesAnim.reserve(pMesh->mNumVertices);
			
			for (_uint j = 0; j < pMesh->mNumVertices; j++)
			{
				Mesh.vecVerticesAnim.push_back(VTXANIMMESH());
			}

			_uint iSize = Mesh.vecVerticesAnim.size();

			for (_uint j = 0; j < iSize; j++)
			{
				memcpy(&Mesh.vecVerticesAnim[j].vPosition, &pMesh->mVertices[j], sizeof(_float3));
				memcpy(&Mesh.vecVerticesAnim[j].vTangent, &pMesh->mTangents[j], sizeof(_float3));
				memcpy(&Mesh.vecVerticesAnim[j].vNormal, &pMesh->mNormals[j], sizeof(_float3));
				memcpy(&Mesh.vecVerticesAnim[j].vTexCoord, &pMesh->mTextureCoords[0][j], sizeof(_float3));

			}

			_uint iBonesNum = pMesh->mNumBones;

			for (_uint j = 0; j < iBonesNum; j++)
			{
				aiBone* pBone = pMesh->mBones[j];

				_float4x4 matOffset;
				memcpy(&matOffset, &pBone->mOffsetMatrix, sizeof(_float4x4));
				XMStoreFloat4x4(&matOffset, XMMatrixTranspose(XMLoadFloat4x4(&matOffset)));

				_uint iBoneIndex = { 0 };

				auto iter = find_if(m_vecBone.begin(), m_vecBone.end(), [&](BONE Bone) {

					if (!strcmp(pBone->mName.data, Bone.szName.c_str()))
						return true;

					++iBoneIndex;

					return false;
				});

				Mesh.vecMeshBoneIndices.push_back(iBoneIndex);

				m_vecBone[iBoneIndex].matOffsetMatrix = matOffset;

				for (_uint k = 0; k < pBone->mNumWeights; k++)
				{
				
					if (Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendWeights.x == 0.f)
					{
						Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendIndices.x = (int32_t)j;
						Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendWeights.x = pBone->mWeights[k].mWeight;
					
					}
					else if (Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendWeights.y == 0.f)
					{
						Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendIndices.y = (int32_t)j;
						Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendWeights.y = pBone->mWeights[k].mWeight;
						
					}
					else if (Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendWeights.z == 0.f)
					{
						Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendIndices.z = (int32_t)j;
						Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendWeights.z = pBone->mWeights[k].mWeight;
					
					}
					else if (Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendWeights.w == 0.f)
					{
						Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendIndices.w = (int32_t)j;
						Mesh.vecVerticesAnim[pBone->mWeights[k].mVertexId].vBlendWeights.w = pBone->mWeights[k].mWeight;

					}
				}
				
			}
		}
		else if (m_eType == TYPE::TYPE_NONANIM)
		{
			Mesh.vecVerticesNonAim.reserve(pMesh->mNumVertices);

			for (_uint j = 0; j < pMesh->mNumVertices; j++)
			{
				Mesh.vecVerticesNonAim.push_back(VTXMESH());
			}

			_uint iSize = Mesh.vecVerticesNonAim.size();

			for (_uint j = 0; j < iSize; j++)
			{
				memcpy(&Mesh.vecVerticesNonAim[j].vPosition, &pMesh->mVertices[j], sizeof(_float3));
				memcpy(&Mesh.vecVerticesNonAim[j].vTangent, &pMesh->mTangents[j], sizeof(_float3));
				memcpy(&Mesh.vecVerticesNonAim[j].vNormal, &pMesh->mNormals[j], sizeof(_float3));
				memcpy(&Mesh.vecVerticesNonAim[j].vTexCoord, &pMesh->mTextureCoords[0][j], sizeof(_float3));

			}
		}

		Mesh.vecIndices.reserve(pMesh->mNumFaces);
		for (_uint j = 0; j < pMesh->mNumFaces;j++)
		{
			_uint3 iIndices = {};

			//memcpy(&iIndices, &pMesh->mFaces[i], sizeof(_uint3));
			iIndices.iX = pMesh->mFaces[j].mIndices[0];
			iIndices.iY = pMesh->mFaces[j].mIndices[1];
			iIndices.iZ = pMesh->mFaces[j].mIndices[2];

			Mesh.vecIndices.push_back(iIndices);
		}

		m_vecMesh.push_back(Mesh);
	}


	return S_OK;
}

HRESULT CConverter_Model::Ready_Materials(const string& strModelFilePath)
{
	m_iMaterialsNum = m_pAiScene->mNumMaterials;

	for (_uint i = 0; i < m_iMaterialsNum; i++)
	{
		MATERIAL Material = {};

		aiMaterial* pMaterial = m_pAiScene->mMaterials[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			_char szDdrive[MAX_PATH] = "";
			_char szDirectory[MAX_PATH] = "";

			_splitpath_s(strModelFilePath.c_str(), szDdrive, MAX_PATH, szDirectory, MAX_PATH,
				nullptr, 0, nullptr, 0);

			aiString	szGetPath;
			if (FAILED(pMaterial->GetTexture(aiTextureType(j), 0, &szGetPath)))
				continue;

			_char	szFileName[MAX_PATH] = "";
			_char	szExc[MAX_PATH] = ".dds";

			_splitpath_s(szGetPath.data, nullptr, 0, nullptr, 0,
				szFileName, MAX_PATH, nullptr, 0);

			_char	szTmp[MAX_PATH] = "";

			strcpy_s(szTmp, szDdrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExc);


			Material.vecMaterialPath[j] = szTmp;
		}

		m_vecMaterial.push_back(Material);
	}

	return S_OK;
}

HRESULT CConverter_Model::Ready_Bones(aiNode* pNode, _int iParentIndex)
{
	/*if (pNode == nullptr)
		return S_OK;*/

	BONE	Bone;

	Bone.szName = pNode->mName.data;
	Bone.iParentIndex = iParentIndex;

	memcpy(&Bone.matTransformation, &pNode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&Bone.matTransformation, XMMatrixTranspose(XMLoadFloat4x4(&Bone.matTransformation)));

	m_vecBone.push_back(Bone);

	_int iParentIndx = m_vecBone.size() - 1;

	for (_uint i = 0; i < pNode->mNumChildren; i++)
	{

		Ready_Bones(pNode->mChildren[i], iParentIndx);
	}

	return S_OK;
}

HRESULT CConverter_Model::Ready_Animation()
{
	m_iAnimationsNum = m_pAiScene->mNumAnimations;

	for (_uint i = 0; i < m_iAnimationsNum; i++)
	{
		ANIMATION Animation = {};

		aiAnimation* pAnimation = m_pAiScene->mAnimations[i];

		Animation.szName = pAnimation->mName.data;
		Animation.fDuration = pAnimation->mDuration;
		Animation.fTicksPerSecond = pAnimation->mTicksPerSecond;

		Animation.vecChannel.reserve(pAnimation->mNumChannels);
		for (_uint j = 0; j < pAnimation->mNumChannels; j++)
			Animation.vecChannel.push_back(CHANNEL());
		
		for (_uint j = 0; j < pAnimation->mNumChannels; j++)
		{

			aiNodeAnim* pChannel = pAnimation->mChannels[j];

			Animation.vecChannel[j].szName = pChannel->mNodeName.data;

			_uint iKeyFrameNum = { 0 };
			iKeyFrameNum = max(pChannel->mNumScalingKeys, pChannel->mNumRotationKeys);
			iKeyFrameNum = max(pChannel->mNumPositionKeys, iKeyFrameNum);

			Animation.vecChannel[j].vecKeyFrame.reserve(iKeyFrameNum);
			for (_uint k = 0; k < iKeyFrameNum; k++)
				Animation.vecChannel[j].vecKeyFrame.push_back(KEYFRAME());

			_float3 vPos;
			_float3 vScale;
			_float4 vRot;

			for (_uint k = 0; k < iKeyFrameNum; k++)
			{
				_float fTime = 0.f;

				if (k < pChannel->mNumScalingKeys)
				{
					memcpy(&vScale, &pChannel->mScalingKeys[k].mValue, sizeof(_float3));
					//Animation.vecChannel[j].vecKeyFrame[k].fTrackPosition = pChannel->mScalingKeys[k].mTime;
					//memcpy(&Animation.vecChannel[j].vecKeyFrame[k].fTrackPosition, &pChannel->mScalingKeys[k].mTime, sizeof(_float));
					fTime = pChannel->mScalingKeys[k].mTime;
				}
				if (k < pChannel->mNumRotationKeys)
				{
					vRot.x = pChannel->mRotationKeys[k].mValue.x;
					vRot.y = pChannel->mRotationKeys[k].mValue.y;
					vRot.z = pChannel->mRotationKeys[k].mValue.z;
					vRot.w = pChannel->mRotationKeys[k].mValue.w;
					//Animation.vecChannel[j].vecKeyFrame[k].fTrackPosition = pChannel->mRotationKeys[k].mTime;
					//memcpy(&Animation.vecChannel[j].vecKeyFrame[k].fTrackPosition, &pChannel->mRotationKeys[k].mTime, sizeof(_float));
					fTime = pChannel->mRotationKeys[k].mTime;
				}
				if (k < pChannel->mNumPositionKeys)
				{
					memcpy(&vPos, &pChannel->mPositionKeys[k].mValue, sizeof(_float3));
					//Animation.vecChannel[j].vecKeyFrame[k].fTrackPosition = pChannel->mPositionKeys[k].mTime;
					//memcpy(&Animation.vecChannel[j].vecKeyFrame[k].fTrackPosition, &pChannel->mPositionKeys[k].mTime, sizeof(_float));
					fTime = pChannel->mPositionKeys[k].mTime;
				}

				
				memcpy(&Animation.vecChannel[j].vecKeyFrame[k].vPosition, &vPos, sizeof(_float3));
				memcpy(&Animation.vecChannel[j].vecKeyFrame[k].vRotation, &vRot, sizeof(_float4));
				memcpy(&Animation.vecChannel[j].vecKeyFrame[k].vScale, &vScale, sizeof(_float3));
				memcpy(&Animation.vecChannel[j].vecKeyFrame[k].fTrackPosition, &fTime, sizeof(_float));

			}
		}

		m_vecAnimation.push_back(Animation);
	}

	return S_OK;
}

CConverter_Model* CConverter_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const string& strModelFilePath)
{
	CConverter_Model* pInstance = new CConverter_Model(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(eType, strModelFilePath))) {
		MSG_BOX("Failed to Created : CConverter_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CConverter_Model::Clone(void* pArg)
{
	CConverter_Model* pInstance = new CConverter_Model(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CConverter_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CConverter_Model::Free()
{
	__super::Free();
}

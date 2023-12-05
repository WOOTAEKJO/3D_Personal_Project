#include "..\Public\Mesh.h"

#include "GameInstance.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CMesh::Initialize_ProtoType(CModel::TYPE eType, const aiMesh* pMesh, _fmatrix matPivot)
{
	m_iMaterialIndex = pMesh->mMaterialIndex;

	strcpy_s(m_cName, pMesh->mName.data);

	m_iVertexBuffersNum = 1;
	m_iVertexNum = pMesh->mNumVertices;
	
	m_iIndexNum = pMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_eIndexForMat =  DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	HRESULT hr = eType == CModel::TYPE::TYPE_ANIM ? Anim_Vertex(pMesh) : NonAnim_Vertex(pMesh, matPivot);

	if (FAILED(hr))
		return E_FAIL;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iIndexNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = 0;
	m_Buffer_Desc.MiscFlags = 0;
	m_Buffer_Desc.StructureByteStride = 0;

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	_uint* pIndex = new _uint[m_iIndexNum];

	_uint iNumIndex = { 0 };

	for (_uint i = 0; i < pMesh->mNumFaces; i++)
	{
		pIndex[iNumIndex++] = pMesh->mFaces[i].mIndices[0];
		pIndex[iNumIndex++] = pMesh->mFaces[i].mIndices[1];
		pIndex[iNumIndex++] = pMesh->mFaces[i].mIndices[2];

		m_vecIndexInfo.push_back(_uint3(pMesh->mFaces[i].mIndices[0],
			pMesh->mFaces[i].mIndices[1], pMesh->mFaces[i].mIndices[2]));
	}

	m_SubResource_Data.pSysMem = pIndex;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	

	Safe_Delete_Array(pIndex);

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

_bool CMesh::Compute_MousePos(_float3* pOut, _matrix matWorld)
{
	_float	fDist = 0.f;

	for (_uint i = 0; i < (m_iIndexNum/3); i++)
	{
		_uint3 iIndices = m_vecIndexInfo[i];

		_vector  vVec1, vVec2, vVec3;

		vVec1 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iX]);
		vVec2 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iY]);
		vVec3 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iZ]);

		if (m_pGameInstance->Intersect(pOut, &fDist, vVec1, vVec2, vVec3, matWorld)) {
			return true;
		}
	}
	return false;
}

HRESULT CMesh::Anim_Vertex(const aiMesh* pMesh)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = 0;							// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXANIMMESH* pVerpostex = new VTXANIMMESH[m_iVertexNum];		// 버텍스 버퍼 안에 들어 갈 값들을 설정해줌

	for (size_t i = 0; i < m_iVertexNum; i++)
	{
		memcpy(&pVerpostex[i].vPosition, &pMesh->mVertices[i], sizeof(_float3));
		m_vecVertexInfo.push_back(pVerpostex[i].vPosition);

		memcpy(&pVerpostex[i].vTangent, &pMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVerpostex[i].vNormal, &pMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVerpostex[i].vTexCoord, &pMesh->mTextureCoords[0][i], sizeof(_float2));

		//m_vecAnimVertexInfo.push_back(pVerpostex[i]);
	}

	for (size_t i = 0; i < pMesh->mNumBones; i++)
	{
		aiBone* pBone = pMesh->mBones[i];

		/* pAIBone->mWeights[j].mVertexId : 이 뼈가 영향을 주는 j번째 정점의 인덱스 */
		for (size_t j = 0; j < pBone->mNumWeights; j++)
		{
			if (pVerpostex[pBone->mWeights[j].mVertexId].vBlendWeights.x == 0.f)
			{
				pVerpostex[pBone->mWeights[j].mVertexId].vBlendIndices.x = (int32_t)i;
				pVerpostex[pBone->mWeights[j].mVertexId].vBlendWeights.x = pBone->mWeights[j].mWeight;
			}
			else if (pVerpostex[pBone->mWeights[j].mVertexId].vBlendWeights.y == 0.f)
			{
				pVerpostex[pBone->mWeights[j].mVertexId].vBlendIndices.y = (int32_t)i;
				pVerpostex[pBone->mWeights[j].mVertexId].vBlendWeights.y = pBone->mWeights[j].mWeight;
			}
			else if (pVerpostex[pBone->mWeights[j].mVertexId].vBlendWeights.z == 0.f)
			{
				pVerpostex[pBone->mWeights[j].mVertexId].vBlendIndices.z = (int32_t)i;
				pVerpostex[pBone->mWeights[j].mVertexId].vBlendWeights.z = pBone->mWeights[j].mWeight;
			}
			else if (pVerpostex[pBone->mWeights[j].mVertexId].vBlendWeights.w == 0.f)
			{
				pVerpostex[pBone->mWeights[j].mVertexId].vBlendIndices.w = (int32_t)i;
				pVerpostex[pBone->mWeights[j].mVertexId].vBlendWeights.w = pBone->mWeights[j].mWeight;
			}
		}
	}

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

	return S_OK;
}

HRESULT CMesh::NonAnim_Vertex(const aiMesh* pMesh, _fmatrix matPivot)
{
	m_iVertexStride = sizeof(VTXMESH);

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = 0;							// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXMESH* pVerpostex = new VTXMESH[m_iVertexNum];		// 버텍스 버퍼 안에 들어 갈 값들을 설정해줌

	for (size_t i = 0; i < m_iVertexNum; i++)
	{
		memcpy(&pVerpostex[i].vPosition, &pMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVerpostex[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVerpostex[i].vPosition), matPivot));

		m_vecVertexInfo.push_back(pVerpostex[i].vPosition);

		memcpy(&pVerpostex[i].vTangent, &pMesh->mTangents[i], sizeof(_float3));

		memcpy(&pVerpostex[i].vNormal, &pMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVerpostex[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVerpostex[i].vNormal), matPivot));

		memcpy(&pVerpostex[i].vTexCoord, &pMesh->mTextureCoords[0][i], sizeof(_float2));

	}

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, const aiMesh* pMesh, _fmatrix matPivot)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(eType, pMesh, matPivot))) {
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	return nullptr;
}

void CMesh::Free()
{
	__super::Free();
}

#include "..\Public\Mesh.h"

#include "GameInstance.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CMesh::Initialize_ProtoType(CModel::TYPE eType, MESH MeshData,
	_fmatrix matPivot, CModel::BONES& pBones)
{
	m_iMaterialIndex = MeshData.iMaterialIndex;

	strcpy_s(m_szName, MeshData.szName.c_str());

	m_iVertexBuffersNum = 1;
	m_iVertexNum = eType == CModel::TYPE::TYPE_ANIM ? MeshData.vecVerticesAnim.size() : MeshData.vecVerticesNonAim.size();
	
	m_iIndexNum = MeshData.vecIndices.size() * 3;
	m_iIndexStride = 4;
	m_eIndexForMat =  DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	HRESULT hr = eType == CModel::TYPE::TYPE_ANIM ? Anim_Vertex(MeshData, pBones) : NonAnim_Vertex(MeshData, matPivot);

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

	for (_uint i = 0; i < MeshData.vecIndices.size(); i++)
	{
		pIndex[iNumIndex++] = MeshData.vecIndices[i].iX;
		pIndex[iNumIndex++] = MeshData.vecIndices[i].iY;
		pIndex[iNumIndex++] = MeshData.vecIndices[i].iZ;

		m_vecIndexInfo.push_back(_uint3(MeshData.vecIndices[i].iX,
			MeshData.vecIndices[i].iY, MeshData.vecIndices[i].iZ));
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

_bool CMesh::Compute_MousePos(_float3* pOut, _matrix matWorld, _float* fDist)
{
	//_float	fDist = 0.f;

	for (_uint i = 0; i < (m_iIndexNum/3); i++)
	{
		_uint3 iIndices = m_vecIndexInfo[i];

		_vector  vVec1, vVec2, vVec3;

		vVec1 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iX]);
		vVec2 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iY]);
		vVec3 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iZ]);

		if (m_pGameInstance->Intersect(pOut, fDist, vVec1, vVec2, vVec3, matWorld)) {
			return true;
		}
	}
	return false;
}

HRESULT CMesh::Bind_Blend(CShader* pShader, const _char* strName, CModel::BONES& pBones, _int iNonBindInd)
{

	_float4x4		BoneMatrices[256];

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/*if (i == iNonBindInd)
		{
			_float fScale = 0.00001f;
			_matrix matNon = XMMatrixScaling(fScale, fScale, fScale);
			XMStoreFloat4x4(&BoneMatrices[i], matNon);

			continue;
		}*/

		XMStoreFloat4x4(&BoneMatrices[i], XMLoadFloat4x4(&m_vecOffsetMatrix[i]) * pBones[m_vecBoneIndices[i]]->Get_CombinedTransformationMatrix());
	}

	return pShader->Bind_Matrixes(strName, BoneMatrices, 256);
}

HRESULT CMesh::Anim_Vertex(MESH MeshData, CModel::BONES& pBones)
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
	ZeroMemory(pVerpostex, sizeof(VTXANIMMESH) * m_iVertexNum);

	for (size_t i = 0; i < m_iVertexNum; i++)
	{
		memcpy(&pVerpostex[i].vPosition, &MeshData.vecVerticesAnim[i].vPosition, sizeof(_float3));
		m_vecVertexInfo.push_back(pVerpostex[i].vPosition);
		
		memcpy(&pVerpostex[i].vTangent, &MeshData.vecVerticesAnim[i].vTangent, sizeof(_float3));
		memcpy(&pVerpostex[i].vNormal, &MeshData.vecVerticesAnim[i].vNormal, sizeof(_float3));
		memcpy(&pVerpostex[i].vTexCoord, &MeshData.vecVerticesAnim[i].vTexCoord, sizeof(_float2));
		memcpy(&pVerpostex[i].vBlendIndices, &MeshData.vecVerticesAnim[i].vBlendIndices, sizeof(XMINT4));
		memcpy(&pVerpostex[i].vBlendWeights, &MeshData.vecVerticesAnim[i].vBlendWeights, sizeof(XMFLOAT4));

	}

	m_iNumBones = MeshData.vecMeshBoneIndices.size();

	for (_uint i = 0; i < m_iNumBones; i++)
	{
		m_vecOffsetMatrix.push_back(pBones[MeshData.vecMeshBoneIndices[i]]->Get_OffsetMatrixt());
		m_vecBoneIndices.push_back(MeshData.vecMeshBoneIndices[i]);
	}

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

	if (m_iNumBones == 0)
	{
		m_iNumBones = 1;	// 강제 갯수 1

		_uint iBoneIndex = { 0 };

		auto iter = find_if(pBones.begin(), pBones.end(), [&](CBone* pBb) {

			if (false == strcmp(m_szName, pBb->Get_BoneName()))
				return true;

			++iBoneIndex;

			return false;

		});

		if (iter == pBones.end())
			return E_FAIL;

		m_vecBoneIndices.push_back(iBoneIndex); // 매쉬의 이름과 같은 이름의 뼈 인덱스 추가

		_float4x4 matOffset;
		XMStoreFloat4x4(&matOffset, XMMatrixIdentity());
		m_vecOffsetMatrix.push_back(matOffset); // Offset 행렬 항등 행렬

	}

	return S_OK;
}

HRESULT CMesh::NonAnim_Vertex(MESH MeshData, _fmatrix matPivot)
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
		memcpy(&pVerpostex[i].vPosition, &MeshData.vecVerticesNonAim[i].vPosition, sizeof(_float3));
		XMStoreFloat3(&pVerpostex[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVerpostex[i].vPosition), matPivot));

		m_vecVertexInfo.push_back(pVerpostex[i].vPosition);

		memcpy(&pVerpostex[i].vTangent, &MeshData.vecVerticesNonAim[i].vTangent, sizeof(_float3));

		memcpy(&pVerpostex[i].vNormal, &MeshData.vecVerticesNonAim[i].vNormal, sizeof(_float3));
		XMStoreFloat3(&pVerpostex[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVerpostex[i].vNormal), matPivot));

		memcpy(&pVerpostex[i].vTexCoord, &MeshData.vecVerticesNonAim[i].vTexCoord, sizeof(_float2));

	}

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, MESH MeshData,
	_fmatrix matPivot, CModel::BONES& pBones)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(eType, MeshData, matPivot, pBones))) {
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

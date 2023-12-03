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

HRESULT CMesh::Initialize_ProtoType(const aiMesh* pMesh, _fmatrix matPivot)
{
	strcpy_s(m_cName, pMesh->mName.data);

	m_iVertexBuffersNum = 1;
	m_iVertexNum = pMesh->mNumVertices;
	m_iVertexStride = sizeof(VTXTBN);

	m_iIndexNum = pMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_eIndexForMat =  DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// �����迭�� ũ��
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;					// ������������ ������������ ����/ ������ ����
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// ���� ���� ����
	m_Buffer_Desc.CPUAccessFlags = 0;							// ������ �ش�
	m_Buffer_Desc.MiscFlags = 0;								// ������ �ش�
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// ���� �ϳ��� ũ�⸦ ���� ����

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXTBN* pVerpostex = new VTXTBN[m_iVertexNum];		// ���ؽ� ���� �ȿ� ��� �� ������ ��������

	for (size_t i = 0; i < m_iVertexNum; i++)
	{ 
		memcpy(&pVerpostex[i].fPosition, &pMesh->mVertices[i],sizeof(_float3));
		XMStoreFloat3(&pVerpostex[i].fPosition, XMVector3TransformCoord(XMLoadFloat3(&pVerpostex[i].fPosition), matPivot));

		memcpy(&pVerpostex[i].fTangent, &pMesh->mTangents[i], sizeof(_float3));

		memcpy(&pVerpostex[i].fBinormal, &pMesh->mBitangents[i], sizeof(_float3));

		memcpy(&pVerpostex[i].fNormal, &pMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVerpostex[i].fNormal, XMVector3TransformNormal(XMLoadFloat3(&pVerpostex[i].fNormal), matPivot));

		memcpy(&pVerpostex[i].fTexCoord, &pMesh->mTextureCoords[0][i], sizeof(_float2));

		m_vecVertexInfo.push_back(pVerpostex[i]);
	}



	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// ���� �Ҵ��� ������ ���۰� �Ҵ��� ������ �����Ͽ� ����
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// ���� �Ҵ��� ���� ���۰� �̹� ����߱⿡ �ʿ� ������ ����

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

		vVec1 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iX].fPosition);
		vVec2 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iY].fPosition);
		vVec3 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iZ].fPosition);

		if (m_pGameInstance->Intersect(pOut, &fDist, vVec1, vVec2, vVec3, matWorld)) {
			return true;
		}
	}
	return false;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pMesh, _fmatrix matPivot)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(pMesh, matPivot))) {
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

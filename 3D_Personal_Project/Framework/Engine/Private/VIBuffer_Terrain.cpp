#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Terrain::Initialize_ProtoType(const wstring& strHeightMapFilePath)
{
	_ulong	dwByte = 0;

	HANDLE	hFile = CreateFile(strHeightMapFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == 0)
		return E_FAIL;

	BITMAPFILEHEADER	fh;
	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);

	BITMAPINFOHEADER	ih;
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	_ulong* pPixel = new _ulong[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	CloseHandle(hFile);

	m_iVertexBuffersNum = 1;
	m_iVertexNum = m_iNumVerticesX * m_iNumVerticesZ;
	m_iVertexStride = sizeof(VTXNORTEX);

	m_iIndexNum = (m_iNumVerticesX-1) * (m_iNumVerticesZ-1) * 2 * 3;
	m_iIndexStride = m_iVertexNum >= 65535 ? 4 : 2;
	m_eIndexForMat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VTXNORTEX* pVertices = new VTXNORTEX[m_iVertexNum];

	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].fPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.f, i);
			pVertices[iIndex].fNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].fTexCoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}

	Safe_Delete_Array(pPixel);

	_uint* pIndices = new _uint[m_iIndexNum];

	_uint iIndexNum = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++) {
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++) {

			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX, iIndex + m_iNumVerticesX +1,
				iIndex + 1, iIndex
			};

			pIndices[iIndexNum++] = iIndices[0];
			pIndices[iIndexNum++] = iIndices[1];
			pIndices[iIndexNum++] = iIndices[2];
			
			XMVECTOR	vVec1, vVec2, vNormal;

			vVec1 = XMLoadFloat3(&pVertices[iIndices[1]].fPosition) - XMLoadFloat3(&pVertices[iIndices[0]].fPosition);
			vVec2 = XMLoadFloat3(&pVertices[iIndices[2]].fPosition) - XMLoadFloat3(&pVertices[iIndices[1]].fPosition);
			vNormal = XMVector3Cross(vVec1, vVec2);

			vNormal = XMLoadFloat3(&pVertices[iIndices[0]].fNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[0]].fNormal, XMVector3Normalize(vNormal));

			vNormal = XMLoadFloat3(&pVertices[iIndices[1]].fNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[1]].fNormal, XMVector3Normalize(vNormal));

			vNormal = XMLoadFloat3(&pVertices[iIndices[2]].fNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[2]].fNormal, XMVector3Normalize(vNormal));

			pIndices[iIndexNum++] = iIndices[0];
			pIndices[iIndexNum++] = iIndices[2];
			pIndices[iIndexNum++] = iIndices[3];

			vVec1 = XMLoadFloat3(&pVertices[iIndices[2]].fPosition) - XMLoadFloat3(&pVertices[iIndices[0]].fPosition);
			vVec2 = XMLoadFloat3(&pVertices[iIndices[3]].fPosition) - XMLoadFloat3(&pVertices[iIndices[2]].fPosition);
			vNormal = XMVector3Cross(vVec1, vVec2);

			vNormal = XMLoadFloat3(&pVertices[iIndices[0]].fNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[0]].fNormal, XMVector3Normalize(vNormal));

			vNormal = XMLoadFloat3(&pVertices[iIndices[2]].fNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[2]].fNormal, XMVector3Normalize(vNormal));

			vNormal = XMLoadFloat3(&pVertices[iIndices[3]].fNormal) + vNormal;
			XMStoreFloat3(&pVertices[iIndices[3]].fNormal, XMVector3Normalize(vNormal));
		}
	}

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = 0;							// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	m_SubResource_Data.pSysMem = pVertices;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iIndexNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = 0;
	m_Buffer_Desc.MiscFlags = 0;
	m_Buffer_Desc.StructureByteStride = 0;

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	m_SubResource_Data.pSysMem = pIndices;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(strHeightMapFilePath))) {
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}

#include "..\Public\VIBuffer_DTerrain.h"
#include "GameInstance.h"



CVIBuffer_DTerrain::CVIBuffer_DTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_DTerrain::CVIBuffer_DTerrain(const CVIBuffer_DTerrain& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_DTerrain::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CVIBuffer_DTerrain::Initialize(void* pArg)
{
	if (pArg != nullptr) {
		DTERRAINDESC* pDTerrainDesc = (DTERRAINDESC*)pArg;
		if (FAILED(Init_Terrain(pDTerrainDesc)))
			return E_FAIL;
	}
	
	return S_OK;
}

void CVIBuffer_DTerrain::Update_Buffer(_fvector fMousePos, _float fRadious, _float fHeight, _float fSharpness)
{
	if (fMousePos.m128_f32[0] < 0)
		return;

	D3D11_MAPPED_SUBRESOURCE	SubResource;
	if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
		return;
	
	XMINT2	iPickIndex = { (_int)(fMousePos.m128_f32[0]), (_int)(fMousePos.m128_f32[2]) };
	_int	iRoundIndex = (_int)(fRadious);

	XMINT2	iBeginIndex, iEndIndex;
	iBeginIndex.x = (0 > (iPickIndex.x - iRoundIndex)) ? 0 : (iPickIndex.x - iRoundIndex);
	iBeginIndex.y = (0 > (iPickIndex.y - iRoundIndex)) ? 0 : (iPickIndex.y - iRoundIndex);
	 
	iEndIndex.x = (_int)m_iNumVerticesX < (iPickIndex.x + iRoundIndex) ? (_int)m_iNumVerticesX : (iPickIndex.x + iRoundIndex);
	iEndIndex.y = (_int)m_iNumVerticesZ < (iPickIndex.y + iRoundIndex) ? (_int)m_iNumVerticesZ : (iPickIndex.y + iRoundIndex);

	for (_int i = iBeginIndex.y; i < iEndIndex.y; i++) {
		for (_int j = iBeginIndex.x; j < iEndIndex.x; j++) {

			_int iIndex = i * m_iNumVerticesX + j;

			_float3	vPos = ((VTXMESH*)(SubResource.pData))[iIndex].vPosition;
			_float fLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPos) - fMousePos));

			if (fRadious < fLength)
				continue;

			//_float fH = fHeight * fabsf(fSharpness - pow((fLength / fRadious), 2.f));
			//_float fH = fHeight * (1.f - pow((fLength / fRadious), 2.f));
			_float fH = fHeight * pow(1.f - (fLength / fRadious), 2.f* fSharpness*3.f);

			if(m_bAdd)
				((VTXMESH*)(SubResource.pData))[iIndex].vPosition.y += fH;
			else
				((VTXMESH*)(SubResource.pData))[iIndex].vPosition.y = fH;

			m_vecVertexInfo[iIndex].vPosition = ((VTXMESH*)(SubResource.pData))[iIndex].vPosition;
		}
	}

	for (_int i = iBeginIndex.y; i < iEndIndex.y; i++) {
		for (_int j = iBeginIndex.x; j < iEndIndex.x; j++) {

			_int iIndex = i * m_iNumVerticesX + j;

			_int iIndices[4] = {
				iIndex + (_int)m_iNumVerticesX, iIndex + (_int)m_iNumVerticesX + 1,
				iIndex + 1, iIndex
			};

			if (i == (m_iNumVerticesZ - 1) || j == (m_iNumVerticesX - 1))
				continue;		// 맨 오른쪽과 맨 위쪽을 예외처리

			// 노멀 벡터(법선 벡터)
			// 처음 초기화한 값이 0.f, 1.f, 0.f 인 것을 잊지말자
			// 나중에 문제 생기면 바꿔줘야함
			_vector	vVec1, vVec2, vNormal;

			vVec1 = XMLoadFloat3(&m_vecVertexInfo[iIndices[1]].vPosition) - XMLoadFloat3(&m_vecVertexInfo[iIndices[0]].vPosition);
			vVec2 = XMLoadFloat3(&m_vecVertexInfo[iIndices[2]].vPosition) - XMLoadFloat3(&m_vecVertexInfo[iIndices[1]].vPosition);
			vNormal = XMVector3Cross(vVec1, vVec2);

			vNormal = XMLoadFloat3(&m_vecVertexInfo[iIndices[0]].vNormal) + vNormal;
			XMStoreFloat3(&m_vecVertexInfo[iIndices[0]].vNormal, XMVector3Normalize(vNormal));
			XMStoreFloat3(&((VTXMESH*)(SubResource.pData))[iIndices[0]].vNormal, XMVector3Normalize(vNormal));

			vNormal = XMLoadFloat3(&m_vecVertexInfo[iIndices[1]].vNormal) + vNormal;
			XMStoreFloat3(&m_vecVertexInfo[iIndices[1]].vNormal, XMVector3Normalize(vNormal));
			XMStoreFloat3(&((VTXMESH*)(SubResource.pData))[iIndices[1]].vNormal, XMVector3Normalize(vNormal));

			vNormal = XMLoadFloat3(&m_vecVertexInfo[iIndices[2]].vNormal) + vNormal;
			XMStoreFloat3(&m_vecVertexInfo[iIndices[2]].vNormal, XMVector3Normalize(vNormal));
			XMStoreFloat3(&((VTXMESH*)(SubResource.pData))[iIndices[2]].vNormal, XMVector3Normalize(vNormal));

			vVec1 = XMLoadFloat3(&m_vecVertexInfo[iIndices[2]].vPosition) - XMLoadFloat3(&m_vecVertexInfo[iIndices[0]].vPosition);
			vVec2 = XMLoadFloat3(&m_vecVertexInfo[iIndices[3]].vPosition) - XMLoadFloat3(&m_vecVertexInfo[iIndices[2]].vPosition);
			vNormal = XMVector3Cross(vVec1, vVec2);

			vNormal = XMLoadFloat3(&m_vecVertexInfo[iIndices[0]].vNormal) + vNormal;
			XMStoreFloat3(&m_vecVertexInfo[iIndices[0]].vNormal, XMVector3Normalize(vNormal));
			XMStoreFloat3(&((VTXMESH*)(SubResource.pData))[iIndices[0]].vNormal, XMVector3Normalize(vNormal));

			vNormal = XMLoadFloat3(&m_vecVertexInfo[iIndices[2]].vNormal) + vNormal;
			XMStoreFloat3(&m_vecVertexInfo[iIndices[2]].vNormal, XMVector3Normalize(vNormal));
			XMStoreFloat3(&((VTXMESH*)(SubResource.pData))[iIndices[2]].vNormal, XMVector3Normalize(vNormal));

			vNormal = XMLoadFloat3(&m_vecVertexInfo[iIndices[3]].vNormal) + vNormal;
			XMStoreFloat3(&m_vecVertexInfo[iIndices[3]].vNormal, XMVector3Normalize(vNormal));
			XMStoreFloat3(&((VTXMESH*)(SubResource.pData))[iIndices[3]].vNormal, XMVector3Normalize(vNormal));

			// 탄젠트 벡터(접선 벡터)
			_float3	vTangent;
			XMStoreFloat3(&vTangent, XMVector3Normalize(XMLoadFloat3(&m_vecVertexInfo[iIndices[2]].vPosition) - XMLoadFloat3(&m_vecVertexInfo[iIndex].vPosition)));
			
			m_vecVertexInfo[iIndex].vTangent = vTangent;
			((VTXMESH*)(SubResource.pData))[iIndex].vTangent = vTangent;

			// 바이 노멀 벡터(종법선 벡터)
			/*_float3	vBinormal;
			XMStoreFloat3(&vBinormal, XMVector3Normalize(XMLoadFloat3(&m_vecVertexInfo[iIndices[0]].vPosition) - XMLoadFloat3(&m_vecVertexInfo[iIndex].vPosition)));

			m_vecVertexInfo[iIndex].fBinormal = vBinormal;
			((VTXMESH*)(SubResource.pData))[iIndex].fBinormal = vBinormal;*/
		}
	}

	m_pContext->Unmap(m_pVB, 0);
}

void CVIBuffer_DTerrain::Compute_MousePos(_float3* pOut, _matrix matWorld)
{
	_float	fEarly = 1000.f;
	_float	fDist = 0.f;
	_float3	fOut = {};

	for (_uint i = 0; i < m_iNumFaces; i++) {
		_uint3 iIndices = m_vecIndexInfo[i];

		
		_vector  vVec1, vVec2, vVec3;

		vVec1 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iX].vPosition);
		vVec2 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iY].vPosition);
		vVec3 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iZ].vPosition);

		if (m_pGameInstance->Intersect(&fOut, &fDist, vVec1, vVec2, vVec3, matWorld)) {
			if (fEarly > fDist)
			{
				fEarly = fDist;

				*pOut = fOut;
			}
		}
	}
}

HRESULT CVIBuffer_DTerrain::Set_Buffer(const _char* strPath)
{
	CMeshData::MESHDATADESC MeshDataDesc;

	MeshDataDesc.eModel_Type = CMeshData::MODEL_TYPE::TERRAIN;
	MeshDataDesc.iNumVertices = m_iVertexNum;
	MeshDataDesc.iVerticesXY.iX = m_iNumVerticesX;
	MeshDataDesc.iVerticesXY.iY = m_iNumVerticesZ;
	MeshDataDesc.iNumFaces = m_iIndexNum / 3;
	MeshDataDesc.vecMeshVertices = m_vecVertexInfo;
	MeshDataDesc.vecIndices = m_vecIndexInfo;
	
	if (FAILED(m_pGameInstance->Save_Data_Mesh(strPath, MeshDataDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_DTerrain::Init_Buffer(CMeshData::MESHDATADESC tMeshData)
{
	m_iNumVerticesX = tMeshData.iVerticesXY.iX;
	m_iNumVerticesZ = tMeshData.iVerticesXY.iY;

	m_iVertexBuffersNum = 1;
	m_iVertexNum = tMeshData.iNumVertices;
	m_iVertexStride = sizeof(VTXMESH);

	VTXMESH* pVertices = new VTXMESH[m_iVertexNum];

	for (_uint i = 0; i < m_iVertexNum; i++)
	{
		pVertices[i].vPosition = tMeshData.vecMeshVertices[i].vPosition;
		pVertices[i].vTangent = tMeshData.vecMeshVertices[i].vTangent;
		pVertices[i].vNormal = tMeshData.vecMeshVertices[i].vNormal;
		pVertices[i].vTexCoord = tMeshData.vecMeshVertices[i].vTexCoord;

		m_vecVertexInfo.push_back(pVertices[i]);
	}
	m_iNumFaces = tMeshData.iNumFaces;
	m_iIndexNum = m_iNumFaces * 3;
	m_iIndexStride = 4;
	m_eIndexForMat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	_uint3* pIndices = new _uint3[tMeshData.iNumFaces];

	for (_uint i = 0; i < tMeshData.iNumFaces; i++)
	{
		pIndices[i] = tMeshData.vecIndices[i];
		m_vecIndexInfo.push_back(pIndices[i]);
	}

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	m_SubResource_Data.pSysMem = pVertices;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iIndexNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

HRESULT CVIBuffer_DTerrain::Init_Terrain(DTERRAINDESC* pDTerrainDesc)
{
	m_iNumVerticesX = pDTerrainDesc->iVerticesXNum;
	m_iNumVerticesZ = pDTerrainDesc->iVerticesZNum;

	m_iVertexBuffersNum = 1;
	m_iVertexNum = m_iNumVerticesX * m_iNumVerticesZ;
	m_iVertexStride = sizeof(VTXMESH);

	VTXMESH* pVertices = new VTXMESH[m_iVertexNum];

	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vTangent = _float3(1.f, 0.f, 0.f);
			//pVertices[iIndex].fBinormal = _float3(0.f, 0.f, 1.f);
			pVertices[iIndex].vNormal = _float3(0.f, 1.f, 0.f);
			pVertices[iIndex].vTexCoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));

			m_vecVertexInfo.push_back(pVertices[iIndex]);
		}
	}
	m_iNumFaces = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iIndexNum = m_iNumFaces * 3;
	m_iIndexStride = 4;//m_iVertexNum >= 65535 ? 4 : 2;
	m_eIndexForMat = DXGI_FORMAT_R32_UINT;//m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	_uint* pIndices = new _uint[m_iIndexNum];

	_uint iIndexNum = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++) {
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++) {

			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX, iIndex + m_iNumVerticesX + 1,
				iIndex + 1, iIndex
			};

			pIndices[iIndexNum++] = iIndices[0];
			pIndices[iIndexNum++] = iIndices[1];
			pIndices[iIndexNum++] = iIndices[2];

			m_vecIndexInfo.push_back(_uint3(iIndices[0], iIndices[1], iIndices[2]));

			pIndices[iIndexNum++] = iIndices[0];
			pIndices[iIndexNum++] = iIndices[2];
			pIndices[iIndexNum++] = iIndices[3];

			m_vecIndexInfo.push_back(_uint3(iIndices[0], iIndices[2], iIndices[3]));

		}
	}

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	m_SubResource_Data.pSysMem = pVertices;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iIndexNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

CVIBuffer_DTerrain* CVIBuffer_DTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_DTerrain* pInstance = new CVIBuffer_DTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CVIBuffer_DTerrain");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

CComponent* CVIBuffer_DTerrain::Clone(void* pArg)
{
	CVIBuffer_DTerrain* pInstance = new CVIBuffer_DTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_DTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_DTerrain::Free()
{
	__super::Free();
}
